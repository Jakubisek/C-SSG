#include <stdio.h>
#include "grid.h"


void get_square(grid_t grid, tile_t *part, size_t index) {
    size_t i = 0;
    size_t counter = 0;
    while (i < 9 && counter < 8) {
        size_t pos = 27*(index / 3) + 3*(index % 3) + (i % 3) + 9*(i / 3);
        if (pos == index) {
            i++;
            continue;
        }
        part[counter++] = grid[pos];
        i++;
    }
}

void get_line(grid_t grid, tile_t *part, size_t index, bool is_row) {
    size_t const start = is_row ? (index % 9) : index - (index % 9);
    size_t counter = 0;
    size_t i = 0;
    while (i < 9 && counter < 8) {
        size_t pos = is_row ? start + i : start + 9*i;
        if (pos == index) {
            i++;
            continue;
        }
        part[counter++] = grid[pos];
        i++;
    }
}

int fill_grid(grid_t grid, char const *data)
{
    for (size_t i = 0; i < 81; i++) {
        if (data[i] == '\0') {
            return i;
        }
        grid[i] = char_to_tile(data[i]);
    }
    return 81;
}

bool update_position(grid_t grid, size_t index)
{
    tile_t row[8];
    tile_t column[8];
    tile_t square[8];

    get_line(grid, row, index, true);
    get_line(grid, column, index, false);
    get_square(grid, square, index);


    return update_tile(row, 8, &grid[index]) || update_tile(column, 8, &grid[index]) || update_tile(square, 8, &grid[index]);
}

void show_grid(grid_t grid)
{
    printf("+---+---+---+ +---+---+---+ +---+---+---+\n");
    for (size_t i = 0; i < 9; i++) {
        for (size_t j = 0; j < 9; j++) {
            printf("| %c ", tile_to_char(grid[9*i+j]));
            if (j % 3 == 2 && j != 8) printf("| ");
        }
        printf("|\n+---+---+---+ +---+---+---+ +---+---+---+\n");
        if (i % 3 == 2 && i != 8) printf("+---+---+---+ +---+---+---+ +---+---+---+\n");
    }
    putchar('\n');
}

