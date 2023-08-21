#include "solver.h"
#include <stdio.h>

#define DEBUGMSG_STACK_CREATION_SUCCESS "Stack was successfully created and loaded with %d numbers.\n"


sstack_t *create_solving_stack(char const *data, size_t initial_depth_limit)
{
    sstack_t *solving_stack = malloc(sizeof(sstack_t));
    if (solving_stack == NULL) return NULL;

    solving_stack->data_array = malloc(sizeof(sgame_t*) * INIT_SSTACK_SIZE);
    if (solving_stack->data_array == NULL) {
        free(solving_stack);
        return NULL;
    }

    solving_stack->max_capacity = 100;
    solving_stack->size = 0;
    solving_stack->data_array[solving_stack->size++] = malloc(sizeof(sgame_t));
    if (solving_stack->data_array[0] == NULL) {
        free(solving_stack->data_array);
        free(solving_stack);
        return NULL;
    }

    int filled = fill_grid(solving_stack->data_array[0]->game_data, data);
    solving_stack->data_array[0]->depth = initial_depth_limit;
    #ifdef DEBUG_MSG
        printf(DEBUGMSG_STACK_CREATION_SUCCESS, filled); 
    #endif

    (void) filled; // stop warning when DEBUG_MSG is not defined
    return solving_stack;
}

int solve(sstack_t *solving_stack, size_t max_solutions_limit)
{
    int solution_counter = 0;

    // do solving

    return solution_counter;
}

void terminate_solving(sstack_t *solving_stack, char const *message, int exit_status)
{
    printf("[SOLVING TERMINATED] Additional information:\n%s", message);
    if (solving_stack == NULL) {
        // PRINT ERROR - could not set up solving stack
        return;
    }

    for (size_t i = 0; i < solving_stack->size; i++) {
        free(solving_stack->data_array[i]);
    }
    free(solving_stack);

    if (exit_status != 0) exit(exit_status);
}
