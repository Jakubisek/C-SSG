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

void get_part(grid_t grid, tile_t **part, enum part_type, size_t index);

size_t part_has_tile(grid_t grid, tile_t tile);

size_t update_all(grid_t grid, size_t x, size_t y);

char tile_as_char(tile_t tile);

void show_grid(grid_t grid);

#endif // GRID_H