#include <stdio.h>
#include <stdlib.h>
#include "grid.h"

int main(int argc, char const *argv[])
{
    if (argc != 2) return EXIT_FAILURE;

    char const *game = argv[1];
    
    tile_t grid[81];
    int nums = fill_grid(grid, game);
    printf("Number of recognized digits: %d\n", nums);
    if (nums < 81) {
        printf("Input does not contain enough digits to display grid\n");
        return EXIT_FAILURE;
    }
    show_grid(grid);
    return EXIT_SUCCESS;
}
