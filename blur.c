#include "filter.h"
#include <stdlib.h>

void blur(uint32_t height, uint32_t width, RGBTRIPLE input[height][width],
    RGBTRIPLE output[height][width], uint32_t size)
{
    const uint32_t BOX_LENGTH = size * 2 + 1;

    /* Optimization attempt 1 */

    RGBTRIPLE **average_grid = calloc(2 * (BOX_LENGTH), sizeof(RGBTRIPLE));
    for (uint32_t i = 0; i < height; ++i)
        for (uint32_t j = 0; j < width; ++j) {
            uint64_t n_subtractions = 0, n_additions = 0;
            for (uint32_t x = i < size ? 0 : i - size,
                          last_x = i + size >= height ? height : i + size;
                 x <= last_x; ++x)
                for (uint32_t y = j < size ? 0 : j - size,
                              last_y = j + size >= width ? width : j + size;
                     y <= last_y; ++y) {
                }
        }

}

