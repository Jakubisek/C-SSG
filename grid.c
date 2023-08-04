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

bool recursive_grid_update(grid_t grid, size_t pos) {
    bool result = false;

    #ifdef DEBUG_MSG
        printf("--- Started update on (%ld, %ld) ---\nTarget: ", pos / 9, pos % 9);
        show_tile(grid[pos]); putchar('\n');
    #endif

    for (size_t is_row = 0; is_row < 2; is_row++)
    {
        size_t start = is_row ? (pos / 9) * 9 : pos % 9;
        for (size_t i = 0; i < 9; i++) {
            size_t current_pos = is_row ? start + i : start + (9 * i);
            if (current_pos == pos || tile_is_solved(grid[current_pos])) {
                continue;
            }
            if (remove_from_tile(&grid[current_pos], grid[pos]) && tile_is_solved(grid[current_pos])) {
                result = true;
                recursive_grid_update(grid, current_pos);
            }
        }
    }
    // remove from the 8 square tiles out of which 4 have already been updated
    size_t square_corner_pos = pos - (pos % 3) - (((pos / 9) % 3) * 9);
    for (size_t i = 0; i < 27; i += 9) {
        for (size_t j = 0; j < 3; j++) {
            size_t current_pos = square_corner_pos + i + j;
            if (current_pos == pos || tile_is_solved(grid[current_pos])) {
                continue;
            }
            if (remove_from_tile(&grid[current_pos], grid[pos]) && tile_is_solved(grid[current_pos])) {
                result = true;
                recursive_grid_update(grid, current_pos);
            }
        }
    }
    return result;
}


bool update_grid(grid_t grid) {
    bool result = false;
    for (size_t i = 0; i < 81; i++) {
        if (tile_is_solved(grid[i])) {
            result |= recursive_grid_update(grid, i);
        }
    }
    return result;
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

