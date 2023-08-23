#include <stdio.h>
#include <stdlib.h>
#include "solver.h"

int main(int argc, char const *argv[])
{
    if (argc != 2) return EXIT_FAILURE;

    sstack_t *solving_stack = create_solving_stack(argv[1], 10);
    if (solving_stack == NULL) {
        fprintf(stderr, "[FATAL ERROR] Could not create solving stack\n");
        return EXIT_FAILURE;
    }

    printf("Initial grid looks like this:\n");
    show_grid(solving_stack->data_array[0]->game_data);

    printf("Solving...\n\n");
    solve(solving_stack, 30);

    terminate_solving(solving_stack, "No problems found during solving.\n", 0);
    return EXIT_SUCCESS;
}