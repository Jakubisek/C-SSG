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
        printf("Performed cycle %d", cycle_count++);

        if (grid_contains_errors(grid)) {
            printf(" - grid is unsolvable\n");
        }

        show_grid(grid);
    }
    
    printf("---\nDone\n");

    if (grid_has_only_solved(grid)) {
        printf("Solved all tiles ");
        printf("%s\n", grid_solved_correctly(grid) ? "with no errors" : " - SOLUTION CONTAINS ERRORS");
    } else {
        printf("but incomplete - not all tiles are solved\n");
    }
    show_grid(grid);

    return EXIT_SUCCESS;
}
