#include <stdio.h>
#include <stdlib.h>
#include "grid.h"

void show_part(grid_t grid, enum part_type type, size_t index) {
    tile_t *part[9];
    get_part(grid, part, type, index);

    for (size_t i = 0; i < 9; i++) {
        printf("| %c ", tile_as_char(*part[i]));
    }
    putchar('|'); putchar('\n');
}

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

    show_part(grid, PART_COLUMN, 2);
    show_part(grid, PART_ROW, 1);
    printf("\nSquares 0 1 2 7 8\n\n");
    show_part(grid, PART_SQUARE, 0);
    show_part(grid, PART_SQUARE, 1);
    show_part(grid, PART_SQUARE, 2);
    show_part(grid, PART_SQUARE, 7);
    show_part(grid, PART_SQUARE, 8);
    
    
    return EXIT_SUCCESS;
}
