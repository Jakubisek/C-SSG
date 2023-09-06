#include "grid.h"
#include <stdlib.h>

#define INIT_SSTACK_SIZE 729

/*
    Solving strategy:
    First determine if the inital grid is solvable, then solve all the hidden and naked singles,
    if this doesn't solve the grid, take the first tile with the least amount of option for number it can contain
    and perform a "fork" - add all the variants of the game into the solving stack effectively solving that tile.

    Next, take the top item of the stack and repeat this process until the stack is empty.
    Whenever a grid is solved or it could be determined that the partial soution is incorect, it should
    be immediately removed from the stack (in case it is validated as correct, solution should be displayed).
*/


/*
    Item of the solving stack,
    represents an unsolved sudoku grid with added information about the depth.
*/
typedef struct sudoku_game {
    tile_t game_data[81]; // all the tiles of the sudoku
    size_t depth; // number of forks necessary to reach this point
} sgame_t;

/*
    Basic dynamically alocated stack that holds data about partially solved grids
    ordered by the number of forks it took to create them (the highest is on the top).
*/
typedef struct solving_stack {
    sgame_t **data_array; // holds unsolved grids
    size_t item_count; // the number of items in data_array
    size_t max_capacity; // the actual allocated size of data_array
} sstack_t;

/**
 * Initializes the solving stack through malloc() and in case of succeedes returns a pointer to it.
 * The size of the stack is explicitly difined as 729 and cannot be exceeded, since that is
 * the maximum number of unsolved games that might be theoretically processed at the same time,
 * it is impossible to exceed this value.
 * 
 * @param data a string for the inital value in the stack (that can be parsed into grid_t)
 * @return A pointer to the solving stack or NULL if dynamic allocation failed.
 * @note INIT_SSTACK_SIZE = 729 because for every tile in grid (=81) thare can be at most 9 forks and 9*81 = 729,
 * @note the actual size of the solving stack should be less then 6 KB when empty
 */
sstack_t *create_solving_stack(char const *data);


/**
 * Solve the contents of the solving stack displaying all solutions found to stdout. If the number of solutions
 * found exceeds max_solution, this function will preemtively end solving (meaning it is possible that not all
 * solutions were found). If this function ecounter a fatal memory failure (malloc() fails, used memory exceeds
 * the set limit), the function will terminate by calling terminate_solving().
 * 
 * @param solving_stack a valid pointer to non-empty solving stack that can be resized by realloc()
 * @param max_solutions_limit the number of found solution will not exceed this value
 * @return The total number of solutions that were verified and displayed.
 */
int solve(sstack_t *solving_stack, size_t max_solutions_limit);


/**
 * Releases all alocated memory of the stack by calling free() on all the elemtent stored.
 * If exit status is set to a non-zero value, this function will call exit() with that value.
 * 
 * @param solving_stack a valid pointer to non-empty solving stack that can be release by calling free()
 * @param message additional information about the reason for calling this function that will be displayed to stdout 
 * @param exit_status value given to exit() or 0 which will prevent calling it (message will be always displayed)
 */
void terminate_solving(sstack_t *solving_stack, char const *message, int exit_status);