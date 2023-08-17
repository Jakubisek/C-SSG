#include "grid.h"

// WIP

int solve(char const *sudoku_to_solve, int *solving_stack, size_t max_solutions);

void setup_solving_stack(size_t size);

void terminate_solving(int *solving_stack, char const *message);