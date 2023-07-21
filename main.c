#include <stdio.h>
#include <stdlib.h>
#include "grid.h"

int main(int argc, char const *argv[])
{
    tile_t tiles[9] = {
        char_to_tile('1'),
        TILE_EMPTY,
        char_to_tile('3'),
        char_to_tile('4'),
        TILE_EMPTY,
        char_to_tile('6'),
        char_to_tile('7'),
        char_to_tile('8'),
        TILE_EMPTY
    };

    show_tile(tiles[4]);
    update_tile(tiles, 9, &tiles[1]);
    update_tile(tiles, 9, &tiles[4]);
    update_tile(tiles, 9, &tiles[8]);

    show_tile(tiles[4]);

    remove_from_tile(&tiles[1], char_to_tile('5'));
    remove_from_tile(&tiles[8], char_to_tile('5'));

    show_tile(tiles[4]);

    update_if_unique(tiles, 9);

    // 5 should be solved
    show_tile(tiles[4]);

    return EXIT_SUCCESS;
}
