#include "grid.h"

// WIP

typedef struct sudoku_game {
    grid_t game_data;
    size_t depth;

    size_t replaced_pos;
    size_t *replaced_values;
    size_t replaced_index;

} game_t;

typedef struct solving_stack {
    game_t **data;
    
    size_t top_index;
    size_t capacity;
    size_t capacity_limit;

} stack_t;

int solve(char const *sudoku_to_solve, stack_t *solving_stack, size_t max_solutions);

stack_t *setup_solving_stack(size_t intial_size, size_t max_allowed_size);

void terminate_solving(stack_t *solving_stack, char const *message);