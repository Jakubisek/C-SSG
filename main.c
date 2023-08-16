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

    show_tile(grid[20]); putchar('\n');
    
    if (grid_contains_errors(grid)) {
        fprintf(stderr, "Grid has no solutions\n---\nDone\n");
        return EXIT_FAILURE;
    }


    int cycle_count = 0;
    while((update_grid(grid) || update_all_unique(grid))) {
        printf("Performed cycle %d", cycle_count++);

        if (grid_contains_errors(grid)) {
            printf(" - grid is unsolvable\n");
        } else putchar('\n');

        show_grid(grid);
    }
    
    printf("---\nDone\n");

    if (grid_has_only_solved(grid)) {
        printf("Solved all tiles ");
        printf("%s\n", verify_solution(grid) ? "with no errors" : " - SOLUTION CONTAINS ERRORS");
    } else {
        printf("but incomplete - not all tiles are solved\n");
    }
    show_grid(grid);

    return EXIT_SUCCESS;
}

// ./run 00608040000700108000045*4*3002051002*4*60905*6*400090020106*4*8003
