#ifndef GRID_H
#define GRID_H

#include "tile.h"

typedef tile_t* grid_t;

enum PART_TYPE {
    PART_ROW,
    PART_COLUMN,
    PART_SQUARE
};

#define PART_TO_STRING(pt)( \
    (pt == PART_ROW) ? "row" : \
    (pt == PART_COLUMN) ? "column" : \
    (pt == PART_SQUARE) ? "square" : \
    "invalid")

int fill_grid(grid_t grid, char const *data);

bool update_grid(grid_t grid);

bool update_all_unique(grid_t grid);

bool grid_has_only_solved(grid_t grid);

bool verify_solution(grid_t grid);

bool grid_contains_errors(grid_t grid);

void show_grid(grid_t grid);

#endif // GRID_H