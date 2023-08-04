#ifndef GRID_H
#define GRID_H

#include "tile.h"

typedef tile_t* grid_t;

int fill_grid(grid_t grid, char const *data);

bool update_grid(grid_t grid);

bool update_all_unique(grid_t grid);

void show_grid(grid_t grid);

#endif // GRID_H