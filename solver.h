#include "grid.h"
#include <stdlib.h>

#define INIT_SSTACK_SIZE 100
#define MAX_USED_MEMORY_LIMIT 100000000 


typedef struct sudoku_game {
    tile_t game_data[81]; // all the tiles of the sudoku
    size_t depth; // the number of remaining forks that can be performed
} sgame_t;


typedef struct solving_stack {
    sgame_t **data_array;
    size_t size;
    size_t max_capacity;
} sstack_t;


// TODO - documentation

/**
 * -
 * @param data 
 * @param initial_depth_limit 
 * @return 
 */
sstack_t *create_solving_stack(char const *data, size_t initial_depth_limit);


/**
 * -
 * @param solving_stack
 * @param max_solution_limit 
 * @return 
 */
int solve(sstack_t *solving_stack, size_t max_solutions_limit);


/**
 * -
 * @param solving_stack 
 * @param message
 * @param exit_status
 */
void terminate_solving(sstack_t *solving_stack, char const *message, int exit_status);