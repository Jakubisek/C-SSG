#ifndef TILE_H
#define TILE_H

#include <stdbool.h>

#define TILE_EMPTY 0x91FF

typedef unsigned long size_t;
typedef unsigned short tile_t;

tile_t char_to_tile(char num);

bool tile_is_solved(tile_t tile);

bool tile_has_num(tile_t tile, tile_t num);

bool add_to_tile(tile_t *tile, tile_t adding);

bool remove_from_tile(tile_t *tile, tile_t tile_to_remove);

bool update_tile(tile_t *tiles, size_t count, tile_t *tile_to_update);

bool update_if_unique(tile_t *tiles, size_t count);

char tile_to_char(tile_t tile);

void show_tile(tile_t tile);

#endif // TILE_H