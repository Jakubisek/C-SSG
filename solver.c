#include "solver.h"
#include <stdio.h>

#define SOLVING_TERMINATED "[SOLVING TERMINATED] Additional information:\n%s"
#define FATAL_MAX_MEMORY_EXCEEDED "Solving was abruptly terminated - memory usage limit for stack was exceeded.\n"
#define FATAL_MALLOC_FAILED "Solving was abruptly terminated - malloc failed to allocate memory.\n"
#define DEBUGMSG_STACK_CREATION_OK "Stack was successfully created and loaded with %d numbers.\n"


sstack_t *create_solving_stack(char const *data, size_t initial_depth_limit)
{
    sstack_t *solving_stack = malloc(sizeof(sstack_t));
    if (solving_stack == NULL) return NULL;

    solving_stack->data_array = malloc(sizeof(sgame_t*) * INIT_SSTACK_SIZE);
    if (solving_stack->data_array == NULL) {
        free(solving_stack);
        return NULL;
    }

    solving_stack->max_capacity = INIT_SSTACK_SIZE;
    solving_stack->item_count = 0;
    solving_stack->data_array[0] = malloc(sizeof(sgame_t));
    if (solving_stack->data_array[0] == NULL) {
        free(solving_stack->data_array);
        free(solving_stack);
        return NULL;
    }

    int filled = fill_grid(solving_stack->data_array[0]->game_data, data);
    solving_stack->data_array[0]->depth = initial_depth_limit;
    #ifdef DEBUG_MSG
        printf(DEBUGMSG_STACK_CREATION_OK, filled); 
    #endif

    (void) filled; // stop a warning when DEBUG_MSG is not defined
    return solving_stack;
}


static void append_to_stack(sstack_t *solving_stack, sgame_t *game_to_append)
{
    if (solving_stack->item_count > MAX_USED_MEMORY_LIMIT) {
        terminate_solving(solving_stack, FATAL_MAX_MEMORY_EXCEEDED, 1);
    }

    if (solving_stack->max_capacity < solving_stack->item_count) {
        solving_stack->data_array[solving_stack->item_count++] = game_to_append;
    }
    sgame_t **new_array = realloc(solving_stack->data_array, solving_stack->max_capacity * 2);
    if (new_array == NULL) terminate_solving(solving_stack, FATAL_MALLOC_FAILED, 1);
    solving_stack->data_array = new_array;
    solving_stack->max_capacity *= 2;
}

/*
    DO NOT use this during fork, just reuse the existing game and append more
*/
static void pop_stack_top(sstack_t *solving_stack) {
    if (solving_stack->item_count == 0) return;
    solving_stack->data_array[--solving_stack->item_count] = NULL;
}



/*
    This fucnction should never receive a solved grid
    doing that should cause segfault but might produce undefined behaviour 
*/
static void fork_stack_top(sstack_t *solving_stack)
{
    if (solving_stack->data_array[solving_stack->item_count]->depth == 0) {
        return;
    }
    grid_t grid = solving_stack->data_array[solving_stack->item_count];

    // assume there is at least on unsolved tile
    tile_t candidate = 0xF000;
    size_t pos = __LONG_MAX__;
    for (size_t i = 0; i < 81; i++) {
        if (tile_is_solved(grid[i])) continue;
        if ((grid[i] TILE_GET_SUM) < (candidate TILE_GET_SUM)) {
            candidate = grid[i];
            pos = i;
        }
    }
    
    for (size_t i = 0; i < 9; i++) {
        if (candidate TILE_GET_SET >> i) {

        }
    }
    
}


int solve(sstack_t *solving_stack, size_t max_solutions_limit)
{
    int solution_counter = 0;

    // do solving

    return solution_counter;
}

void terminate_solving(sstack_t *solving_stack, char const *message, int exit_status)
{
    printf(SOLVING_TERMINATED, message);
    if (solving_stack == NULL) {
        // PRINT ERROR - could not set up solving stack
        return;
    }

    for (size_t i = 0; i < solving_stack->item_count; i++) {
        free(solving_stack->data_array[i]);
    }
    free(solving_stack->data_array);
    free(solving_stack);

    if (exit_status != 0) exit(exit_status);
}
