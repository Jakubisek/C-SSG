#include "solver.h"
#include <stdio.h>

#define SOLVING_TERMINATED "[SOLVING TERMINATED] Additional information:\n%s"
#define NUMBER_OF_SOLUTION_EXCEEDED "Number of solutions found exceeds the given limit - cannot continue solving.\n"
#define DEBUGMSG_STACK_CREATION_OK "[INFO] Stack was successfully created and loaded with %d numbers.\n"
#define INFO_SOLUTION_FOUND "[INFO] Found and verified solution %d\n(required %ld additional numbers):\n"
#define INFO_ALL_SOLUTIONS_LISTED "[INFO] All valid solutions were found - no other are possible\n"
#define WARNING_STACK_NULL "[WARNING] Solution stack was incorrectly initalized and could not be freed.\n"
#define FATAL_MAX_MEMORY_EXCEEDED "Solving was abruptly terminated - memory usage limit for stack exceeded.\n"
#define FATAL_MALLOC_FAILED "Solving was abruptly terminated - malloc failed to allocate memory.\n"
#define IMPOSSIBLE "The impossible became possible!\n"

#define SSTACK_TOP solving_stack->item_count - 1


sstack_t *create_solving_stack(char const *data)
{
    sstack_t *solving_stack = malloc(sizeof(sstack_t));
    if (solving_stack == NULL) return NULL;

    solving_stack->data_array = malloc(sizeof(sgame_t*) * INIT_SSTACK_SIZE);
    if (solving_stack->data_array == NULL) {
        free(solving_stack);
        return NULL;
    }

    solving_stack->data_array[0] = malloc(sizeof(sgame_t));
    if (solving_stack->data_array[0] == NULL) {
        free(solving_stack->data_array);
        free(solving_stack);
        return NULL;
    }
    solving_stack->max_capacity = INIT_SSTACK_SIZE;

    int filled = fill_grid(solving_stack->data_array[0]->game_data, data);
    
    solving_stack->data_array[0]->depth = 0;
    solving_stack->item_count = 1;
    #ifdef DEBUG_MSG
        printf(DEBUGMSG_STACK_CREATION_OK, filled); 
    #endif

    (void) filled; // stop a warning when DEBUG_MSG is not defined
    return solving_stack;
}


static void append_to_stack(sstack_t *solving_stack, sgame_t *game_to_append)
{
    if (solving_stack->max_capacity > solving_stack->item_count) {
        solving_stack->data_array[solving_stack->item_count++] = game_to_append;
        return;
    }
    terminate_solving(solving_stack, IMPOSSIBLE, 1);
}


static void pop_stack_top(sstack_t *solving_stack)
{
    if (solving_stack->item_count == 0) return;
    free(solving_stack->data_array[SSTACK_TOP]);
    solving_stack->data_array[--solving_stack->item_count] = NULL;
}


static void fork_stack_top(sstack_t *solving_stack)
{
    
    solving_stack->data_array[SSTACK_TOP]->depth++;
    grid_t grid = solving_stack->data_array[SSTACK_TOP]->game_data;

    // assume there is at least one unsolved tile
    tile_t candidate = 0xF000;
    size_t pos = __LONG_MAX__;
    for (size_t i = 0; i < 81; i++) {
        if (tile_is_solved(grid[i])) continue;
        if ((grid[i] TILE_GET_SUM) < (candidate TILE_GET_SUM)) {
            candidate = grid[i];
            pos = i;
        }
    }
    
    size_t fork_current_index = SSTACK_TOP;
    for (size_t i = 0; i < (tile_t)(((candidate TILE_GET_SUM) >> 12) - 1); i++) {
        
        sgame_t *new_game = malloc(sizeof(sgame_t));
        if (new_game == NULL) terminate_solving(solving_stack, FATAL_MALLOC_FAILED, 1);
        *new_game = *(solving_stack->data_array[fork_current_index]);
        append_to_stack(solving_stack, new_game);
    }

    for (size_t i = 0; i < 9; i++) {
        if ((((candidate TILE_GET_SET) >> i) & 1) == 0) continue;
        solving_stack->data_array[fork_current_index++]->game_data[pos] = char_to_tile('1' + i);
    }
}


int solve(sstack_t *solving_stack, size_t max_solutions_limit)
{
    int solution_counter = 0;

    // maybe add proper warnings for this
    if (solving_stack == NULL || max_solutions_limit == 0) return 0; 

    update_grid(solving_stack->data_array[SSTACK_TOP]->game_data);
    if (grid_contains_errors(solving_stack->data_array[SSTACK_TOP]->game_data)) {
        #ifndef DEBUG_MSG
            printf("The initial grid data contians a trivial mistake - grid has no solutions.\n");
            printf("(re-run with debug messages to see details)\n\n");
        #endif
        return 0;
    }

    while (solving_stack->item_count > 0) {
        grid_t grid = solving_stack->data_array[SSTACK_TOP]->game_data;
        bool found_errors = false;
        while((update_grid(grid) || update_all_unique(grid))) {
            if (grid_contains_errors(grid)) {
                found_errors = true;
                break;
            } 
        }
        if (found_errors) {
            pop_stack_top(solving_stack);
            continue;
        }

        if (verify_solution(grid)) {
            if (solution_counter == max_solutions_limit) {
                terminate_solving(solving_stack, NUMBER_OF_SOLUTION_EXCEEDED , 1);
            }
            printf(INFO_SOLUTION_FOUND, solution_counter++, solving_stack->data_array[SSTACK_TOP]->depth);
            show_grid(grid);
            pop_stack_top(solving_stack);
            continue;
        }
        if (!grid_has_only_solved(grid)) {
            fork_stack_top(solving_stack);
        }
    }
    return solution_counter;
}

void terminate_solving(sstack_t *solving_stack, char const *message, int exit_status)
{
    if (solving_stack->item_count == 0) printf(INFO_ALL_SOLUTIONS_LISTED);
    printf(SOLVING_TERMINATED, message);
    if (solving_stack == NULL) {
        printf(WARNING_STACK_NULL);
        return;
    }

    for (size_t i = 0; i < solving_stack->item_count; i++) {
        free(solving_stack->data_array[i]);
    }
    free(solving_stack->data_array);
    free(solving_stack);

    if (exit_status != 0) exit(exit_status);
}
