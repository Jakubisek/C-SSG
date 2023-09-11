#include <stdio.h>
#include <stdlib.h>
#include "solver.h"

static bool parse_ulong(const char *value_to_parse, size_t *result_value) {
    char c;
    size_t counter = 0; 
    size_t new_value = 0;
    while ((c = value_to_parse[counter++]) != '\0') {
        if (c < '0' || c > '9') return false;
        new_value = new_value * 10 + c - '0';
    }
    *result_value = new_value;
    return true;
}

int main(int argc, char const *argv[])
{
    if (argc > 3 || argc < 2) {
        fprintf(stderr, "Unexpected number of arguments: %d\n", argc - 1);
        fprintf(stderr, "Expected: 1. (optional) solution limit, 2. (required) sudoku data.\n");
        fprintf(stderr, "Example: ./cssg 14 *669107*7*55*4359*541*27*46*3205*491*4404006007*590308\n");
        return EXIT_FAILURE;
    }

    // change the default solution_limit
    size_t solution_limit = 3;

    if (argc == 3 && !parse_ulong(argv[1], &solution_limit)) {
        printf("'%s' was not accepted as value for solution display limit - using default value instead.\n", argv[1]);
    }

    sstack_t *solving_stack = create_solving_stack(argv[argc-1]);
    if (solving_stack == NULL) {
        fprintf(stderr, "Solving stack could not be initialized - cannot solve.\n");
        return EXIT_FAILURE;
    }

    printf("Initial grid looks like this:\n");
    show_grid(solving_stack->data_array[0].game_data);

    printf("Solving...\n\n");
    solve(solving_stack, solution_limit);

    terminate_solving(solving_stack, 0);

    return EXIT_SUCCESS;
}