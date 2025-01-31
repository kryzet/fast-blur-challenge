#ifndef FILTER_H
#define FILTER_H

#include <stdint.h>

/* -------------------------------------------------------------------------- */

// BMP-related data types based on Microsoft's own

// These data types are essentially aliases for C/C++ primitive data types. 
// Adapted from http://msdn.microsoft.com/en-us/library/cc230309.aspx.
// See https://en.wikipedia.org/wiki/C_data_types#stdint.h for more on stdint.h.

typedef uint8_t  BYTE;
typedef uint32_t DWORD;
typedef int32_t  LONG;
typedef uint16_t WORD;

// The BITMAPFILEHEADER structure contains information about the type, size,
// and layout of a file that contains a DIB [device-independent bitmap].
// Adapted from http://msdn.microsoft.com/en-us/library/dd183374(VS.85).aspx.

typedef struct
{
    WORD   bfType;
    DWORD  bfSize;
    WORD   bfReserved1;
    WORD   bfReserved2;
    DWORD  bfOffBits;
} __attribute__((__packed__))
BITMAPFILEHEADER;

// The BITMAPINFOHEADER structure contains information about the
// dimensions and color format of a DIB [device-independent bitmap].
// Adapted from http://msdn.microsoft.com/en-us/library/dd183376(VS.85).aspx.

typedef struct
{
    DWORD  biSize;
    LONG   biWidth;
    LONG   biHeight;
    WORD   biPlanes;
    WORD   biBitCount;
    DWORD  biCompression;
    DWORD  biSizeImage;
    LONG   biXPelsPerMeter;
    LONG   biYPelsPerMeter;
    DWORD  biClrUsed;
    DWORD  biClrImportant;
} __attribute__((__packed__))
BITMAPINFOHEADER;

// The RGBTRIPLE structure describes a color consisting of relative intensities of
// red, green, and blue. Adapted from http://msdn.microsoft.com/en-us/library/aa922590.aspx.

#pragma pack(1)
typedef struct
{
    BYTE  rgbtBlue;
    BYTE  rgbtGreen;
    BYTE  rgbtRed;
}
RGBTRIPLE;
#pragma pack()

/* -------------------------------------------------------------------------- */

/*
 * Box-blur image and store the result in output.
 *
 * image is expected to point to a region of memory with pixel data for
 * (height * width) RGBTRIPLE's.
 * output is expected to point to a writable region of memory with a size of
 * height * width * sizeof(RGBTRIPLE).
 * size denotes the box blur radius.
 */
void blur(uint32_t height, uint32_t width, RGBTRIPLE input[height][width], RGBTRIPLE output[height][width], uint32_t size);

#endif /* FILTER_H */
