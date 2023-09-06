#include <stdio.h>
#include <stdlib.h>
#include "solver.h"
#include "messages.h"

int main(int argc, char const *argv[])
{
    if (argc != 2) return EXIT_FAILURE;

    arg_options_t options;
    options.show_errors = true;
    options.show_warnings = true;
    options.show_info = true;
    options.use_color = true;

    show_error(options, E_MALOC_FAILED, 2, "name", "surname");
    show_error(options, E_MEMORY_EXCEEDED, 0);
    show_warning(options, W_INPUT_TOO_LONG, 2, '0', 0);
    show_warning(options, W_MISTAKE_IN_INPUT, 0);
    show_info(options, I_ALL_OK, 0);
    show_info(options, I_SOLUTION_FOUND, 1, 5);

    sstack_t *solving_stack = create_solving_stack(argv[1]);
    if (solving_stack == NULL) {
        fprintf(stderr, "[FATAL ERROR] Could not create solving stack\n");
        return EXIT_FAILURE;
    }

    printf("Initial grid looks like this:\n");
    show_grid(solving_stack->data_array[0]->game_data);

    printf("Solving...\n\n");
    solve(solving_stack, 20);

    terminate_solving(solving_stack, "No problems found during solving.\n", 0);
    return EXIT_SUCCESS;
}