#ifndef GRID_H
#define GRID_H

enum part_type {
    PART_ROW,
    PART_COLUMN,
    PART_SQUARE
};

typedef unsigned long size_t;
typedef unsigned short tile_t;

tile_t num_to_tile(char num);

tile_t remove_from_tile(tile_t *tile, char num);



typedef tile_t* grid_t;

int fill_grid(grid_t grid, char const *data);

size_t update_all(grid_t grid, unsigned long x, unsigned long y);

tile_t **get_part(grid_t grid, enum part_type, unsigned long index);

size_t part_has_tile(grid_t grid, tile_t tile);

#endif // GRID_H