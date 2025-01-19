#define _DEFAULT_SOURCE
#define _POSIX_C_SOURCE 199309L

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "filter.h"
#include "blur.c"

#define BLUR_SIZE 3

struct benchmark {
    struct timespec before_wall, after_wall, delta_wall;
#ifdef TIME_CPU
    struct timespec before_cpu, after_cpu, delta_cpu;
#endif
};

double timespec_diff(struct timespec *a, struct timespec *b, struct timespec *d);

int main( int argc, char *argv[] )
{
    BITMAPFILEHEADER bf = { 0x4D42, 0, 0, 0, sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) };
    BITMAPINFOHEADER bi = { sizeof(BITMAPINFOHEADER), 0, 0, 1, 24, 0, 0, 11811, 11811, 0, 0 };
    int width, height, num_channels;
    int64_t n = 1;

    // check for correct number of arguments
    if ( argc != 3 && argc != 4 )
    {
        fprintf( stderr, "Usage: %s inputfile outputfile [n]\n"
            "\tn: Number of times to run benchmark [optional]\n",
            argv[0] != NULL ? argv[0] : "filter"
        );
        exit( EXIT_FAILURE );
    }

    if ( argc == 4 )
    {
        // note: we don't care if input starts numerically and ends with text;
        // if we find a valid value we will use it
        n = strtol(argv[3], 0, 10);
        if ( n == LONG_MIN || n == LONG_MAX || n < 1 || n > 10000 )
        {
            fprintf( stderr, "Invalid n; expected [1 - 10000]\n" );
            exit( EXIT_FAILURE );
        }
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

    struct benchmark *bench = malloc( n * sizeof(struct benchmark) );
    if ( bench == NULL )
    {
        fprintf( stderr, "Error allocating memory for benchmark data!\n" );
        exit( EXIT_FAILURE );
    }

    // run benchmark
    for ( uint32_t i = 0; i < n; ++i )
    {
        // note: \r used here to overwrite the line on each run
        fprintf( stderr, "Running blur...%d/%ld\r", i + 1, n );
        clock_gettime( CLOCK_MONOTONIC, &bench[i].before_wall );
#ifdef TIME_CPU
        clock_gettime( CLOCK_PROCESS_CPUTIME_ID, &bench[i].before_cpu );
#endif

        // perform the actual function call to blur
        blur( height, width, (RGBTRIPLE(*)[width])input, output, BLUR_SIZE );

        clock_gettime( CLOCK_MONOTONIC, &bench[i].after_wall );
#ifdef TIME_CPU
        clock_gettime( CLOCK_PROCESS_CPUTIME_ID, &bench[i].after_cpu );
#endif
    }
    // note: the trailing spaces overwrite the tail end of n when at it's max length
    fprintf( stderr, "Running blur...finished.  \n" );

    // print timing data
    for ( uint32_t i = 0; i < n; ++i )
    {
        // calculate difference between starting time and ending time
        double delta_wall_d = timespec_diff( &bench[i].before_wall, &bench[i].after_wall, &bench[i].delta_wall );
        fprintf
        (
            stderr,
            "Elapsed wall time: %.7f [%ld:%ld] seconds\n",
            delta_wall_d, bench[i].delta_wall.tv_sec, bench[i].delta_wall.tv_nsec
        );

#ifdef TIME_CPU
        double delta_cpu_d = timespec_diff( &bench[i].before_cpu, &bench[i].after_cpu, &bench[i].delta_cpu );
        fprintf
        (
            stderr,
            "Elapsed cpu time: %.7f [%ld:%ld] seconds\n",
            delta_cpu_d, bench[i].delta_cpu.tv_sec, bench[i].delta_cpu.tv_nsec
        );
#endif
    }

    // free input image
    stbi_image_free( input );

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

double
timespec_diff(struct timespec *a, struct timespec *b, struct timespec *d)
{
    double delta;
    uint64_t delta_s, delta_ns;
    delta_s = b->tv_sec - a->tv_sec;
    if (b->tv_nsec >= a->tv_nsec) {
        delta_ns = b->tv_nsec - a->tv_nsec;
    } else {
        delta_s--;
        delta_ns = 1000000000 - (a->tv_nsec - b->tv_nsec);
    }
    d->tv_sec = delta_s;
    d->tv_nsec = delta_ns;
    delta = delta_s + delta_ns / 1000000000.0;
    return delta;
}
