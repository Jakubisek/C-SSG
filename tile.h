#ifndef TILE_H
#define TILE_H

#include <stdbool.h>

typedef unsigned long size_t;
typedef unsigned short tile_t;

#define TILE_EMPTY 0x91FF
#define TILE_ERROR 0

/*
    tile_t
    Acts like a set that can contain the values 1-9 and also contains information
    on how many elements are stored in this set

    out of the 16 bits:
    first 4 store the number of elements in the set (sum)
    the remaining 12 are used to store the values (elements)

    TILE_EMPTY contains a full set, that means 9 elements
    sum  000 elements 
    1001 0001 1111 1111
    = 0x91FF
*/

#define TILE_GET_SUM &0xF000
#define TILE_GET_SET &0x01FF


/**
 * Converts a char into a tile_t.
 * '0' is converted to TILE_EMPTY.
 * '1' to '9' are converted to their respective solved tiles.
 * Otherwise, TILE_ERROR is returned.
 *
 * @param num the character to convert.
 * @return the corresponding tile_t value (with sum always equal to either 0, 1, or 9).
 */
tile_t char_to_tile(char num);

/**
 * Checks if a tile has only one option for a number.
 *
 * @param tile the tile to check.
 * @return true if the sum of tile is equal to 1, false otherwise.
 * @warning only checks the sum bits and ignores the rest, this might return true for invalid tiles
 */
bool tile_is_solved(tile_t tile);


/**
 * Checks if a tile contains all the elements from another tile.
 *
 * @param tile the tile to check
 * @param num the number to look for in the tile.
 * @return true if `num` is a subset of `tile`, false otherwise.
 */
bool tile_has_num(tile_t tile, tile_t num);


/**
 * Performs a set union and addition of sum bits adding `tile_to_add` to `tile`.
 *
 * @param tile a pointer to the tile to update.
 * @param tile_to_add the tile to add to the original (first) tile.
 * @return true if the operation changes the value of `tile`, false otherwise.
 * @warning ignores sum bits of both parameters recalcualting a new value after addition
 */
bool add_to_tile(tile_t *tile, tile_t tile_to_add);


/**
 * Performs a set subtraction, removing `tile_to_remove` from `tile`.
 *
 * @param tile a pointer to the tile to update.
 * @param tile_to_remove the tile to remove from the original (first) tile.
 * @return true if the operation changes the value of `tile`, false otherwise.
 * @warning ignores sum bits of both parameters recalcualting a new value after subtraction
 */
bool remove_from_tile(tile_t *tile, tile_t tile_to_remove);


/**
 * Updates `tile_to_update` by removing all the values from `tiles` which are solved.
 *
 * @param tiles an array of tiles to compare with.
 * @param count the number of tiles in the array.
 * @param tile_to_update A pointer to the tile to update.
 * @return true if the operation changes the value of `tile_to_update`, false otherwise.
 * @note including a tile with the same address as `tile_to_update` in tiles will cause it to be ignored
 */
bool remove_all_solved(tile_t *tiles, size_t count, tile_t *tile_to_update);


/**
 * Updates (only solves) all the tiles from `tiles` that contain a unique value in that array.
 *
 * @param tiles an array of tiles to check.
 * @param count the number of tiles in the array.
 * @return true if any tiles are updated, false otherwise.
 * @note in a situation where multiple values in tile are unique, the highest will be chosen
 */
bool solve_if_unique(tile_t *tiles, size_t count);


/**
 * Converts a tile_t to a character.
 *
 * @param tile The tile to convert.
 * @return The character representation of the tile.
 * @note '!' means the tile is equal to TILE_ERROR or 0
 * @note ' ' means the tile is equal to TILE_EMTPY or 0x91FF
 * @note '?' means the tile is not solved but also not equal to TILE_EMPTY
 * @note 'X' means the tile format was not recognized as valid
 */
char tile_to_char(tile_t tile);


/**
 * TEMP - Prints a more readable binary representation of a tile to stdout
 * @note syntax (dec)sum-(bin)elements
 */
void show_tile(tile_t tile);

#endif // TILE_H