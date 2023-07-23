#ifndef GRID_H
#define GRID_H

#include "tile.h"

typedef tile_t* grid_t;

int fill_grid(grid_t grid, char const *data);

bool update_position(grid_t grid, size_t index);

void show_grid(grid_t grid);

#endif // GRID_H