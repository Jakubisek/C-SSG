#include <stdio.h>
#include "grid.h"

enum PART_TYPE {
    PART_ROW,
    PART_COLUMN,
    PART_SQUARE
};
const char *const pt[] = {"row", "column", "square"};


static void get_part(grid_t grid, tile_t **part, size_t index, enum PART_TYPE type)
{
    if (type == PART_SQUARE) {
        for (size_t i = 0; i < 9; i++) {
            size_t pos = 27*(index / 3) + 3*(index % 3) + (i % 3) + 9*(i / 3);
            part[i] = &grid[pos];
        }
        return;
    }
    size_t const start = (type == PART_ROW) ? 9 * index : index;
    for (size_t i = 0; i < 9; i++) {
        size_t pos = (type == PART_ROW) ? start + i : start + 9*i;
        part[i] = &grid[pos];
    }
}

static size_t pos_from_part(enum PART_TYPE part_type, size_t index, size_t order)
{
    if (part_type == PART_ROW) return index*9 + order;
    if (part_type == PART_COLUMN) return index + order*9;

    return 27*(index / 3) + 3*(index % 3) + (order % 3) + 9*(order / 3);
}


int fill_grid(grid_t grid, char const *data)
{
    bool count_zeros = false;
    size_t char_counter = 0;
    size_t grid_index = 0;

    char c;
    while ((c = data[char_counter++]) != '\0') {

        if (grid_index >= 81) {
            printf("Warning: input contains redundant data, ignoring everythinf from '%c' at %ld.\n", c, char_counter - 1);
            break;
        }
        if (c == '*') {
            if (count_zeros) printf("Grid loading: ignored redundant * expansion at %ld.\n", char_counter - 1);
            count_zeros = true;
            continue;
        }
        if (c > '9' || c < '0') {
            // skip warning for grid characters
            if (c == ' ' || c == '|' || c == '-' || c == '+' || c == '.') {
                continue;
            }
            printf("Grid loading: ignored unexpected character '%c' at %ld.\n", c, char_counter - 1);
            continue;
        }
        if (!count_zeros) {
            grid[grid_index++] = char_to_tile(c);
            continue;
        }

        count_zeros = false;
        for (size_t i = 0; (i < c - '0') && (grid_index + i < 81); i++) grid[grid_index + i] = TILE_EMPTY;

        // special case for 10 zeros
        if (c == '0') {
            for (size_t i = 0; (i < 10) && (grid_index + i < 81); i++) grid[grid_index + i] = TILE_EMPTY;
            grid_index += 10;
        }
        grid_index += c - '0';
    }
    if (grid_index < 81) {
        for (size_t i = 0; i < 81 - grid_index; i++) {
            grid[grid_index + i] = TILE_EMPTY;
        }
    }
    return (grid_index > 81) ? 81 : grid_index;
}


static bool recursive_grid_update(grid_t grid, size_t pos)
{
    
    if (!tile_is_solved(grid[pos])) return false;
    bool result = false;

    for (int part_type = 0; part_type < 3; part_type++) {
        size_t start = 
            (part_type == PART_SQUARE) ? pos - (pos % 3) - (((pos / 9) % 3) * 9) :
            (part_type == PART_ROW) ? ((pos / 9) * 9) : pos % 9;

        for (size_t i = 0; i < 9; i++) {
            size_t current_pos = 
                (part_type == PART_SQUARE) ? start + ((i / 3) * 9) + (i % 3) :
                (part_type == PART_ROW) ? start + i : start + (9 * i);

            if (current_pos == pos) {
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


bool update_grid(grid_t grid)
{
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
            if (!solve_if_unique(part, 9)) {
                continue;
            }
            result = true;
            for (size_t j = 0; j < 9; j++) {
                recursive_grid_update(grid, pos_from_part((enum PART_TYPE)part_type, i, j));
            }
        }
    }
    return result;
}

bool grid_has_only_solved(grid_t grid)
{
    for (size_t i = 0; i < 81; i++) {
        if (!tile_is_solved(grid[i])) {
            return false;
        }
    }
    return true;
}


bool verify_solution(grid_t grid)
{
    if (!grid_has_only_solved(grid)) return false;
    tile_t *part[9];
    tile_t empty_tester;
    for (size_t i = 0; i < 9; i++) {
        for (int part_type = 0; part_type < 3; part_type++) {
            get_part(grid, part, i, (enum PART_TYPE) part_type);
            empty_tester = TILE_EMPTY;
            remove_all_solved(part, 9, &empty_tester);
            if (empty_tester != TILE_ERROR) {
                return false;
            }
        }
    }
    return true;
}


bool grid_contains_errors(grid_t grid, bool explain_error)
{
    for (size_t i = 0; i < 81; i++) {
        if (grid[i] == TILE_ERROR) {
            if (!explain_error) return true;
            printf("Grid contained duplicit values which were removed and marked with '!'.\n");
            return true;
        }
    }
    tile_t *part[9];

    tile_t empty_tester;
    tile_t sum_tester;

    for (size_t i = 0; i < 9; i++) {
        for (int part_type = 0; part_type < 3; part_type++) {
            get_part(grid, part, i, (enum PART_TYPE) part_type);
            empty_tester = TILE_EMPTY;
            sum_tester = TILE_ERROR;
            for (size_t j = 0; j < 9; j++) {
                add_to_tile(&sum_tester, *part[j]);
                if (!tile_is_solved(*part[j])) continue;
                if (!remove_from_tile(&empty_tester, *part[j])) {
                    printf("Could not complete %s %ld, pos %ld.\n", pt[part_type], i, j);
                    return true;
                }
            }
            if ((sum_tester TILE_GET_SET) != (TILE_EMPTY TILE_GET_SET)) {
                if (!explain_error) return true;
                printf("Could not complete %s %ld, missing: ", pt[part_type], i);
                tile_t missing_tile = ((9 << 12) - (sum_tester TILE_GET_SUM)) | ((~sum_tester) TILE_GET_SET);
                show_tile(missing_tile);
                printf("%s %c\n", tile_is_solved(missing_tile) ? " aka" : "", tile_to_char(missing_tile));
                return true;
            }
        }
    }
    return false;
}

const char *const row_line = "+---+---+---+ +---+---+---+ +---+---+---+\n";

void show_grid(grid_t grid)
{
    #ifdef HIDE_SOLUTONS
    return;
    #endif

    #ifdef COMPACT_SOLUTIONS

    for (size_t i = 0; i < 81; i++) {
        putchar(tile_to_char(grid[i]));
    }
    putchar('\n');
    return;

    #else

    // normal grid display
    printf("%s",row_line);
    for (size_t i = 0; i < 9; i++) {
        for (size_t j = 0; j < 9; j++) {
            printf("| %c ", tile_to_char(grid[9*i+j]));
            if (j % 3 == 2 && j != 8) printf("| ");
        }
        printf("|\n%s", row_line);
        if (i % 3 == 2 && i != 8) printf("%s", row_line);
    }
    putchar('\n');

    #endif
}
