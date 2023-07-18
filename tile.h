#ifndef TILE_H
#define TILE_H

#include <stdbool.h>

typedef unsigned long size_t;
typedef unsigned short tile_t;

tile_t char_to_tile(char num);

bool tile_is_solved(tile_t tile);

bool tile_has_num(tile_t tile, tile_t contains);

bool add_to_tile(tile_t *tile, tile_t adding);

bool remove_from_tile(tile_t *tile, tile_t removing);

bool update_tile(tile_t *tiles, size_t count, tile_t *updating);

bool update_if_unique(tile_t *tiles, size_t count);

#endif // TILE_H