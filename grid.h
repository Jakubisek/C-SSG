#ifndef GRID_H
#define GRID_H

#include "tile.h"

/*
    This represents an array of size 81 of tile_t, if grid_t is a pointer to an array of
    a different size, the behaviour of functions that use grid_t is undefined.
*/
typedef tile_t* grid_t;


/**
 * Fills the entire grid with data from a null terminated string. Numbers (if found) are loaded into the grid
 * from left to right, top to bottom. If less than 81 numbers are provided, (counted after * expansion),
 * the rest of the grid will be filled with TILE_EMPTY ignoring the initial values,
 * if more than 81 are found, the additional characters are ignored.
 * 
 * @param grid will be rewritten with data
 * @param data string with data to fill grid (all non-numeric character in data will be ignored)
 * @return the number of valid numeric characters that was found in data
 * @note a sequence *x in data where x is a number expands to x successive zeros (12*434 -> 12000034)
 * @note special case with 0 (...*0) will expand to 10 zeros
 */
int fill_grid(grid_t grid, char const *data);


/**
 * Updates all tiles in the grid so that they contain only those values that could be used to solve that tile
 * without sharing a row/column/square with the same value.
 * 
 * @param grid might be updated (only targets tiles that are not solved)
 * @return true if this operation resulted in any changes in grid, false otherwise
 * @note update will never result in creation of TILE_ERROR anywhere because solved tiles will never be edited
 */
bool update_grid(grid_t grid);


/**
 * Updates those tiles that contain a unique value in a row/column/square, if any tile is solved during this,
 * the grid will be immediately updated so that the values are removed from relevant rows, columns, or squares.
 * 
 * @param grid might be updated druing this operation
 * @return true if this operation resulted in any changes in grid, false otherwise
 */
bool update_all_unique(grid_t grid);


/**
 * Checks if the given grid only contains solved tiles, solving should be terminated immediatly after
 * this function returns true, but the solution still needs to be checked for rule violations.
 * 
 * @param grid will not be modified during this operation
 * @return true if there are no unsolved tiles, false otherwise (TILE_ERROR is not considered as solved)
 */
bool grid_has_only_solved(grid_t grid);


/**
 * Verifies that all rows, columns, and squares contain no duplicate values, if that is not the case,
 * the invalid part and index will be printed to stderr with the number that is missing from that part,
 * (if multiple different numbers are missing no number will be shown).
 * 
 * @param grid will not be modified during this operation
 * @return true if the solution contains no invalid parts and all tiles are solved, false otherwise
 * @note the invalid part will only be printed to stderr if debug messages are enabled
 */
bool verify_solution(grid_t grid);


/**
 * Tries to preemptively determine if there exists a valid solution for the given grid, but might return
 * false negatives - complicated situations might still be considered "solvable" with false being returned.
 * This function expects that the grid might contain unsolved tiles, but if it does not, it will return the same
 * result as !verify_solution().
 * 
 * @param grid will not be modified during this operation
 * @return true if there no problems were found, false otherwise
 * @note similarly to verify_solution(), the invalid part is printed to stderr only if debug messages are enabled
 */
bool grid_contains_errors(grid_t grid);


/**
 * Prints a visual representation of the grid to stdout using tile_to_char() to represent the tiles.
 * @param grid will not be modified during this operation
 */
void show_grid(grid_t grid);

#endif // GRID_H