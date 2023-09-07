#include <stdio.h>
#include <stdlib.h>
#include "solver.h"
#include "messages.h"

int main(int argc, char const *argv[])
{
    if (!parse_options(argc, argv)) {
        show_error(E_ARGUMENT_PARSING_FAILED, 0);
        return EXIT_FAILURE;
    }

    sstack_t *solving_stack = create_solving_stack(argv[argc-1]);
    if (solving_stack == NULL) {
        show_error(E_STACK_INIT_FAILED, 0);
        return EXIT_FAILURE;
    }

    printf("Initial grid looks like this:\n");
    show_grid(solving_stack->data_array[0]->game_data);

    printf("Solving...\n\n");
    solve(solving_stack);

    terminate_solving(solving_stack, 0);
    if (parse_options)

    return EXIT_SUCCESS;
}