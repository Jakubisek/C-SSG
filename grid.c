#include <stdio.h>
#include "grid.h"
#include "messages.h"

enum PART_TYPE {
    PART_ROW,
    PART_COLUMN,
    PART_SQUARE
};

#define PART_TO_STR(pt)( \
    (pt == PART_ROW) ? "row" : \
    (pt == PART_COLUMN) ? "column" : \
    (pt == PART_SQUARE) ? "square" : \
    "invalid")


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
            show_warning(W_INPUT_TOO_LONG, 2, c, char_counter - 1);
            break;
        }
        if (c == '*') {
            if (count_zeros) show_warning(W_EMPTY_ZERO_EXPANSION, 1, char_counter - 1);
            count_zeros = true;
            continue;
        }
        if (c > '9' || c < '0') {
            show_warning(W_CHAR_IGNORED, 2, c, char_counter - 1);
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

    debug_msg("update on pos (%ld, %ld)", 2, pos / 9, pos % 9);

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
                debug_msg("verification failed on: %s %ld", 2, PART_TO_STR(part_type), i);
                return false;
            }
        }
    }
    return true;
}

bool grid_contains_errors(grid_t grid)
{
    for (size_t i = 0; i < 81; i++) {
        if (grid[i] == TILE_ERROR) {
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
                if (!remove_from_tile(&empty_tester, *part[j])) return true;
            }
            if ((sum_tester TILE_GET_SET) != (TILE_EMPTY TILE_GET_SET)) return true;
        }
    }
    return false;
}

const char *const row_line = "+---+---+---+ +---+---+---+ +---+---+---+\n";

void show_grid(grid_t grid)
{
    if (!parsed_options.display_solution) return;

    if(parsed_options.compact_solution) {
        for (size_t i = 0; i < 81; i++) {
            putchar(tile_to_char(grid[i]));
        }
        putchar('\n');
        return;
    }

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
}
