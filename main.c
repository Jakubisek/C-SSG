#include <stdio.h>
#include <stdlib.h>
#include "grid.h"

#define INVALID_MODE "Failed: Invalid syntax\nCannot recognize mode: %s\n"
#define INVALID_SYNTAX "Failed: Invalid syntax\nExpected arguments: mode game (in this order)\n"

enum mode {
    MODE_PLAIN_TXT,
    MODE_GRAPHICS,
    MODE_INTERACTIVE
};

int get_options(int argc, char const **argv, enum mode *mode, char const **game) {
    if (argc > 3 || argc <= 1) {
        fprintf(stderr, INVALID_SYNTAX);
        return EXIT_FAILURE;
    }
    if (argc == 3) {
        if (argv[argc - 2][0] != '-' || argv[argc - 2][1] == '\0') {
            fprintf(stderr, INVALID_MODE, argv[argc - 2]);
            return EXIT_FAILURE;
        }
        switch (argv[argc - 2][1]) {
            case 't': case 'T':
                *mode = MODE_PLAIN_TXT; break;

            case 'g': case 'G':
                *mode = MODE_GRAPHICS; break;

            case 'i': case 'I':
                *mode = MODE_INTERACTIVE; break;

            default:
                fprintf(stderr, INVALID_MODE, argv[argc - 2]);
                return EXIT_FAILURE;
        }
        if (argv[argc - 2][2] != '\0') {
            fprintf(stderr, INVALID_MODE, argv[argc - 2]);
            return EXIT_FAILURE;
        }
    }

    *game = argv[argc - 1];
    return EXIT_SUCCESS;
}

void show_grid(grid_t grid) {
    printf("+---+---+---+ +---+---+---+ +---+---+---+\n");
    for (size_t i = 0; i < 9; i++) {
        for (size_t j = 0; j < 9; j++) {
            printf("| %c ", (char)((grid[9*i+j] & 0xf000) >> 12) + '0');
            if (j % 3 == 2 && j != 8) printf("| ");
        }
        printf("|\n+---+---+---+ +---+---+---+ +---+---+---+\n");
        if (i % 3 == 2 && i != 8) printf("+---+---+---+ +---+---+---+ +---+---+---+\n");
    }
    putchar('\n');
}

int main(int argc, char const *argv[])
{
    char const *game = NULL;
    enum mode mode = MODE_PLAIN_TXT; 
    if (get_options(argc, argv, &mode, &game) == EXIT_FAILURE) return EXIT_FAILURE;

    printf("Mode %d, Game: %s\n", mode, game);
    
    tile_t grid[81];
    int nums = fill_grid(grid, game);
    printf("Counted: %d\n", nums);
    if (nums == 81) show_grid(grid);

    return EXIT_SUCCESS;
}
