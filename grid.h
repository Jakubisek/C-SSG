#ifndef GRID_H
#define GRID_H

#include "tile.h"

typedef tile_t* grid_t;

int fill_grid(grid_t grid, char const *data);

bool update_grid(grid_t grid);

bool update_all_unique(grid_t grid); // TODO

bool grid_is_solved(grid_t grid);

bool grid_is_correct(grid_t grid); // TODO

void show_grid(grid_t grid);

#endif // GRID_H