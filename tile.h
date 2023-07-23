#ifndef TILE_H
#define TILE_H

#include <stdbool.h>

typedef unsigned long size_t;
typedef unsigned short tile_t;

#define TILE_EMPTY 0x91FF

/*
    tile_t
    acts like a set that can contain the values 1-9 and also contains information
    on how many elements are stored in this set

    out of the 16 bits:
    first 4 store the number of elements in the set (sum)
    the remaining 12 are used to store the values (elements)

    TILE_EMPTY contains a full set, that means 9 elements
    sum  000 elements 
    1001 0001 1111 1111
    = 0x91FF
*/

/*
    Converts char into tile_t
    '0' is converted to TILE_EMPTY
    '1' to '9' are converted to their respective solved tiles
    otherwise 0 tile is returned
*/
tile_t char_to_tile(char num);

/*
    Checks if a tile has only one option for a number,
    this is done by checking if the sum == 1
*/
bool tile_is_solved(tile_t tile);

/*
    Checks if a tile contains `num`
    @warning ingores the sum of both parameters

    @returns true if `num` is a subset of `tile`, false otherwise
*/
bool tile_has_num(tile_t tile, tile_t num);

/*
    Performs a set union and adds `tile_to_add` to `tile`
    @returns false if operation does not change the value of `tile`, true otherwise
    @attention recalculates sum after the operation ignoring the old values
*/
bool add_to_tile(tile_t *tile, tile_t tile_to_add);

/*
    Preforms a set subtraction removing `tile_to_remove` from `tile`
    @returns false if operation does not change the value of `tile`, true otherwise
    @attention recalculates sum after the operation ignoring the old values
*/
bool remove_from_tile(tile_t *tile, tile_t tile_to_remove);

/*
    Updates `tile_to_update` by removing all the values which are solved in `tile`
    @returns false if operation does not change the value of `tile_to_update`, true otherwise
    @attention since remove_from_tile() is used, sum might be updated during this operaiton
    @warning including `tile_to_update` in `tiles` will cause this tile to be ignored only if they have the same adress
*/
bool update_tile(tile_t *tiles, size_t count, tile_t *tile_to_update);

/*
    Updates all the tiles from `tiles` that contain a unique value
    @returns false if no tiles are updated, true otherwise
    @attention if a situation occurs where multiple values are unique, the largest will be chosen
*/
bool update_if_unique(tile_t *tiles, size_t count);

/*
    An inverse function to char_to_tile()
    @returns
    '?' if the tile is not solved,
    '!' if `tile` contains error,
    '1' - '9' othrwise
*/
char tile_to_char(tile_t tile);

/*
    More readable binary representation of a tile with sum in decimal
    @note TEMP, syntax (dec)sum-(bin)elements
*/
void show_tile(tile_t tile);

#endif // TILE_H