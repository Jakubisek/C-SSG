#ifndef GRID_H
#define GRID_H

enum part_type {
    PART_ROW,
    PART_COLUMN,
    PART_SQUARE
};

typedef unsigned int tile_t;

tile_t num_to_tile(char num);

char remove_from_tile(tile_t tile, char num);



typedef tile_t* grid_t;

grid_t make_grid(char const *data);

int update_all(grid_t grid, unsigned long x, unsigned long y);

tile_t *get_part(grid_t grid, enum part_type, unsigned long index);

int part_has_tile(grid_t grid, tile_t tile);


#endif // GRID_H