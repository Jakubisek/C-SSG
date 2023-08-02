#include <stdio.h>
#include <stdlib.h>
#include "grid.h"

int main(int argc, char const *argv[])
{
    tile_t grid[81];
    int filled = fill_grid(grid, argv[1]);
    printf("---\nLoaded %d numbers into grid\n\n", filled);
    show_grid(grid);

    for (size_t i = 0; i < 81; i++) {
        update_position(grid, i);
    }

    printf("---\nDone\n\n");
    show_grid(grid);

    show_tile(grid[1]); putchar(' ');
    show_tile(grid[2]); putchar('\n');
    return EXIT_SUCCESS;
}
