#include "filter.h"
#include <math.h>

void blur(uint32_t height, uint32_t width, RGBTRIPLE input[height][width],
          RGBTRIPLE output[height][width], uint32_t size)
{
    const uint32_t BOX_LENGTH = size * 2 + 1;

    // Used for indexing into sums[]
    enum { RED, GREEN, BLUE };

    /* Optimization attempt 2 */
    // ========================================
    RGBTRIPLE row_sum[BOX_LENGTH];
    for (uint32_t i = 0; i < BOX_LENGTH; ++i)
        row_sum[i] = input[0][i];
    // ========================================

    for (uint32_t i = 0; i < height; ++i)
        for (uint32_t j = 0; j < width; ++j) {
            int sums[3] = {0};
            float counter = 0;
            // Obtain needed pixels from copy[][]
            for (uint32_t x = i < size ? 0 : i - size,
                          last_x = i + size >= height ? width : i + size;
                 x <= last_x; ++x)
                for (uint32_t y = j < size ? 0 : j - size,
                              last_y = j + size >= width ? width : j + size;
                     y <= last_y; ++y) {
                    sums[RED] += input[x][y].rgbtRed;
                    sums[GREEN] += input[x][y].rgbtGreen;
                    sums[BLUE] += input[x][y].rgbtBlue;
                    ++counter;
                }
            for (int k = 0; k < 3; ++k)
                sums[k] = fmin(round(sums[k] / counter), UINT8_MAX);
            output[i][j].rgbtRed = sums[RED];
            output[i][j].rgbtGreen = sums[GREEN];
            output[i][j].rgbtBlue = sums[BLUE];
        }
}
