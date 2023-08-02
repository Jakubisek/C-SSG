#include <stdio.h>
#include "grid.h"


void get_square(grid_t grid, tile_t *part, size_t index) {
    for (size_t i = 0; i < 9; i++) {
        size_t pos = 27*(index / 3) + 3*(index % 3) + (i % 3) + 9*(i / 3);
        part[i] = grid[pos];
    }
}

void get_line(grid_t grid, tile_t *part, size_t index, bool is_row) {
    size_t const start = is_row ? (index % 9) : index - (index % 9);
    for (size_t i = 0; i < 9; i++) {
        size_t pos = is_row ? start + i : start + 9*i;
        part[i] = grid[pos];
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

bool update_position(grid_t grid, size_t pos)
{
    if (tile_is_solved(grid[pos])) return false;
    tile_t before_update = grid[pos];

    size_t row_start = pos / 9;
    for (size_t i = 0; i < 9; i++) {
        size_t current_pos = row_start + i;
        if (current_pos != pos && tile_is_solved(grid[current_pos])) {
            remove_from_tile(&grid[pos], grid[current_pos]);
        }
    }

    size_t column_start = pos % 9;
    for (size_t i = 0; i < 9; i++) {
        size_t current_pos = column_start + (9 * i);
        if (current_pos != pos && tile_is_solved(grid[current_pos])) {
            remove_from_tile(&grid[pos], grid[current_pos]);
        }
    }

    // SQUARE UPDATE IS HARD
    // size_t square_corner_pos = pos - (pos % 3) - (((pos / 9) % 3) * 9);
    // for (size_t i = 0; i < 4; i++) {
    //     remove_from_tile(&grid[pos], grid[square_corner_pos + (18 * (i / 2) + 2 * (i % 2))]);
    // }

    return before_update == grid[pos];
}

const char *const line = "+---+---+---+ +---+---+---+ +---+---+---+\n";

void show_grid(grid_t grid)
{
    printf(line);
    for (size_t i = 0; i < 9; i++) {
        for (size_t j = 0; j < 9; j++) {
            printf("| %c ", tile_to_char(grid[9*i+j]));
            if (j % 3 == 2 && j != 8) printf("| ");
        }
        printf("%s%s", "|\n", line);
        if (i % 3 == 2 && i != 8) printf(line);
    }
    putchar('\n');
}

