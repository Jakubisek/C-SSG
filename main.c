#include <stdio.h>
#include <stdlib.h>
#include "grid.h"

int main(int argc, char const *argv[])
{
    tile_t grid[81];
    int filled = fill_grid(grid, argv[1]);
    printf("---\nLoaded %d numbers into grid\n\n", filled);
    show_grid(grid);

    int cycle_count = 0;
    while((update_grid(grid) || update_all_unique(grid))) {
        printf("Performed cycle %d\n", cycle_count++);
        show_grid(grid);
        show_tile(grid[28]); putchar('\t'); show_tile(grid[29]); putchar('\n');
    }
    
    printf("---\nDone\n");

    if (grid_is_solved(grid)) {
        printf("Solved all tiles\n");
        if (!grid_is_correct(grid)) printf("SOLUTION CONTAINS ERRORS\n"); else printf("no errors\n");
    } else {
        printf("but incomplete - not all tiles are solved\n");
    }
    show_grid(grid);

    return EXIT_SUCCESS;
}
