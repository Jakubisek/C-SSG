#include <stdio.h>
#include "grid.h"


void get_part(grid_t grid, tile_t **part, size_t index, enum PART_TYPE type) {
    if (type == PART_SQUARE) {
        for (size_t i = 0; i < 9; i++) {
            size_t pos = 27*(index / 3) + 3*(index % 3) + (i % 3) + 9*(i / 3);
            part[i] = &grid[pos];
        }
        return;
    }
    size_t const start = (type == PART_ROW) ? index - (index % 9) : (index % 9);
    for (size_t i = 0; i < 9; i++) {
        size_t pos = (type == PART_ROW) ? start + i : start + 9*i;
        part[i] = &grid[pos];
    }
}

size_t pos_from_part(enum PART_TYPE part_type, size_t index, size_t order) {
    if (part_type == PART_ROW) {
        return index - (index % 9) + order;
    }
    if (part_type == PART_COLUMN) {
        return (index % 9) + order*9;
    }
    return 27*(index / 3) + 3*(index % 3) + (order % 3) + 9*(order / 3);
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

/*
    TODO: make this readable using pos_from_part()
*/
bool recursive_grid_update(grid_t grid, size_t pos) {
    
    if (!tile_is_solved(grid[pos])) return false;
    bool result = false;

    #ifdef DEBUG_MSG
        printf("--- Started update on (%ld, %ld) --- tile ", pos / 9, pos % 9);
        show_tile(grid[pos]); putchar('\n');
    #endif

    for (size_t is_row = 0; is_row < 2; is_row++) {
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
    /*  
        remove from the 8 square tiles out of which 4 have already been updated
        checking which were and were not updated would be slower than just doing it twice
    */
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

bool update_all_unique(grid_t grid)
{
    bool result = false;
    tile_t *part[9];
    for (size_t i = 0; i < 9; i++) {
        for (int part_type = 0; part_type < 3; part_type++) {
            get_part(grid, part, i, (enum PART_TYPE) part_type);
            if (solve_if_unique(part, 9)) {
                result = true;
                for (size_t j = 0; j < 9; j++) {
                    recursive_grid_update(grid, pos_from_part((enum PART_TYPE)part_type, i, j));
                }
            }
        }
    }
    return result;
}

bool grid_is_solved(grid_t grid)
{
    for (size_t i = 0; i < 81; i++) {
        if (!tile_is_solved(grid[i])) {
            return false;
        }
    }
    return true;
}

bool grid_is_correct(grid_t grid)
{
    tile_t *part[9];
    tile_t empty_tester;
    for (size_t i = 0; i < 9; i++) {
        for (int part_type = 0; part_type < 3; part_type++) {
            get_part(grid, part, i, (enum PART_TYPE) part_type);
            empty_tester = TILE_EMPTY;
            remove_all_solved(part, 9, &empty_tester);
            if (empty_tester != TILE_ERROR) {
                #ifdef DEBUG_MSG
                    printf("%s -%ld- CONTAINS AN ERROR\n", PART_TO_STRING(part_type), i);
                    show_tile(empty_tester); putchar('\n');
                #endif
                return false;
            }
        }
    }
    return true;
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

