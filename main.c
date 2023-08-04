#include <stdio.h>
#include <stdlib.h>
#include "grid.h"

int main(int argc, char const *argv[])
{
    tile_t grid[81];
    int filled = fill_grid(grid, argv[1]);
    printf("---\nLoaded %d numbers into grid\n\n", filled);
    show_grid(grid);

    update_grid(grid);

    printf("---\nDone\n\n");
    show_grid(grid);

    show_tile(grid[3]); putchar(' ');
    show_tile(grid[4]); putchar('\n');
    return EXIT_SUCCESS;
}
