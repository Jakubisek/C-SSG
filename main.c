#include <stdio.h>
#include <stdlib.h>
#include "solver.h"
#include "messages.h"

int main(int argc, char const *argv[])
{
    if (argc != 2) return EXIT_FAILURE;

    show_error(E_MALOC_FAILED, 2, "name", "surname");
    show_error(E_MEMORY_EXCEEDED, 0);
    show_warning(W_INPUT_TOO_LONG, 2, '0', 0);
    show_warning(W_MISTAKE_IN_INPUT, 0);
    show_info(I_ALL_OK, 0);
    show_info(I_SOLUTION_FOUND, 2, 1, 5);

    printf("\n ... \n\n");

    sstack_t *solving_stack = create_solving_stack(argv[1]);
    if (solving_stack == NULL) {
        fprintf(stderr, "[FATAL ERROR] Could not create solving stack\n");
        return EXIT_FAILURE;
    }

    printf("Initial grid looks like this:\n");
    show_grid(solving_stack->data_array[0]->game_data);

    printf("Solving...\n\n");
    solve(solving_stack);

    terminate_solving(solving_stack, 0);
    return EXIT_SUCCESS;
}