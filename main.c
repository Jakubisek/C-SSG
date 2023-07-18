#include <stdio.h>
#include <stdlib.h>
#include "grid.h"

int main(int argc, char const *argv[])
{
    tile_t empty_tile = TILE_EMPTY;
    tile_t five_tile = char_to_tile('5');

    show_tile(empty_tile);
    show_tile(five_tile);

    if (tile_is_solved(five_tile)) {
        remove_from_tile(&empty_tile, five_tile);
        printf("After removing five from empty:\n");
        putchar(tile_to_char(empty_tile)); putchar(' ');
        show_tile(empty_tile);
    }

    return EXIT_SUCCESS;
}
