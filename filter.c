#define _DEFAULT_SOURCE

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/time.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "filter.h"
#include "blur.c"

#define BLUR_SIZE 3

int main( int argc, char *argv[] )
{
    BITMAPFILEHEADER bf = { 0x4D42, 0, 0, 0, sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) };
    BITMAPINFOHEADER bi = { sizeof(BITMAPINFOHEADER), 0, 0, 1, 24, 0, 0, 11811, 11811, 0, 0 };
    int width, height, num_channels;
    struct rusage before, after;
    struct timeval total_before, total_after, difference;

    // check for correct number of arguments
    if ( argc != 3 )
    {
        fprintf( stderr, "Usage: %s [inputfile] [outputfile]\n", argv[0] != NULL ? argv[0] : "filter" );
        exit( EXIT_FAILURE );
    }

    // open and decode input file
    unsigned char *input = stbi_load( argv[1], &width, &height, &num_channels, 3 );
    if ( input == NULL )
    {
        fprintf( stderr, "Error opening/decoding input file!\n" );
        exit( EXIT_FAILURE );
    }

    // change RGB channel order to BGR
    for ( int i = 0; i < width * height * 3; i += 3 )
    {
        unsigned char temp = input[i];
        input[i]   = input[i+2];
        input[i+2] = temp;
    }

    // allocate memory output bitmap data
    RGBTRIPLE(*output)[width] = calloc( height * width, sizeof(RGBTRIPLE) );
    if ( output == NULL )
    {
        fprintf( stderr, "Error allocating memory for output image data!\n" );
        exit( EXIT_FAILURE );
    }

    // perform the actual function call to blur
    fprintf( stderr, "Calling function blur..." );
    getrusage( RUSAGE_SELF, &before );
    blur( height, width, (RGBTRIPLE(*)[width])input, output, BLUR_SIZE );
    getrusage( RUSAGE_SELF, &after );
    fprintf( stderr, "finished.\n" );

    // free input image
    stbi_image_free( input );

    // calculate difference between starting time and ending time
    timeradd( &before.ru_utime, &before.ru_stime, &total_before );
    timeradd( & after.ru_utime, & after.ru_stime, &total_after  );
    timersub( &total_after, &total_before, &difference );

    // print rusage data
    fprintf
    (
        stderr,
        "Elapsed time: %.5f seconds\n",
        ( difference.tv_sec * 1000 * 1000 + difference.tv_usec ) / ( 1000.0 * 1000.0)
    );

    // open output file
    FILE *fp = fopen( argv[2], "wb" );
    if ( fp == NULL )
    {
        fprintf( stderr, "Error opening output file!\n" );
        exit( EXIT_FAILURE );
    }

    // calculating amount of padding
    int padding = ( 4 - ( width * sizeof(RGBTRIPLE) ) % 4 ) % 4;

    // write headers to output file
    bf.bfSize = bf.bfOffBits + ( ( sizeof(RGBTRIPLE) * width ) + padding ) * height;
    bi.biWidth = width;
    bi.biHeight = -height;
    fwrite( &bf, sizeof bf, 1, fp );
    fwrite( &bi, sizeof bi, 1, fp );

    // write bitmap data to output file
    for ( int i = 0; i < height; i++ )
    {
        fwrite( output[i], sizeof(RGBTRIPLE), width, fp );

        // add padding
        for ( int j = 0; j < padding; j++ )
        {
            putc( 0x00, fp );
        }
    }

    // close output file
    fclose(fp);

    // free output bitmap data
    free( output );
}
