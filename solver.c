#include "solver.h"
#include <stdio.h>
#include "messages.h"

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

    debug_msg("finished stack init size = %d", INIT_SSTACK_SIZE);

    return solving_stack;
}


static void append_to_stack(sstack_t *solving_stack, sgame_t *game_to_append)
{
    if (solving_stack->max_capacity > solving_stack->item_count) {
        solving_stack->data_array[solving_stack->item_count++] = game_to_append;
        return;
    }
    show_error(E_IMPLICIT_ASSERTION_FAILED, 1, "append_to_stack");
    terminate_solving(solving_stack, 1);
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
        if (new_game == NULL) {
            show_error(E_MALOC_FAILED, 2, "fork_stack_top", "struct sgame_t");
            terminate_solving(solving_stack, 1);
        }
        *new_game = *(solving_stack->data_array[fork_current_index]);
        append_to_stack(solving_stack, new_game);
    }

    for (size_t i = 0; i < 9; i++) {
        if ((((candidate TILE_GET_SET) >> i) & 1) == 0) continue;
        solving_stack->data_array[fork_current_index++]->game_data[pos] = char_to_tile('1' + i);
    }
}


int solve(sstack_t *solving_stack)
{
    int solution_counter = 0;

    // maybe add proper warning for this
    if (solving_stack == NULL) return 0; 

    update_grid(solving_stack->data_array[SSTACK_TOP]->game_data);
    if (grid_contains_errors(solving_stack->data_array[SSTACK_TOP]->game_data)) {
        show_warning(W_MISTAKE_IN_INPUT, 0);
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
            if (solution_counter > parsed_options.solution_limit) {
                show_error(E_SOLUTIONS_EXCEEDED, 0);
                terminate_solving(solving_stack, 1);
            }
            show_info(I_SOLUTION_FOUND, 2, solution_counter++, solving_stack->data_array[SSTACK_TOP]->depth);

            // TODO: handle show_grid() based on user preferences
            if (parsed_options.display_solution) show_grid(grid);

            pop_stack_top(solving_stack);
            continue;
        }
        if (!grid_has_only_solved(grid)) {
            fork_stack_top(solving_stack);
        }
    }
    return solution_counter;
}

void terminate_solving(sstack_t *solving_stack, int exit_status)
{
    if (solving_stack->item_count == 0 && exit_status == 0) show_info(I_ALL_OK, 0);

    if (solving_stack == NULL) {
        debug_msg("(!) stack was NULL at exit: %d", exit_status);
        return;
    }

    for (size_t i = 0; i < solving_stack->item_count; i++) {
        free(solving_stack->data_array[i]);
    }
    free(solving_stack->data_array);
    free(solving_stack);

    if (exit_status != 0) exit(exit_status);
}
