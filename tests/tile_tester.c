#include "../tile.h"
#include <string.h>


int main(int argc, char const *argv[])
{
    if (argc != 3) {
        return 2;
    }
    
    size_t size = strlen(argv[1]);
    tile_t tiles[size];
    tile_t *tested_tile = NULL;

    for (size_t i = 0; i < size; i++) {
        tiles[i] = char_to_tile(argv[1][i]);
        if (argv[1][i] == '?') {
            tiles[i] = TILE_EMPTY;
            tested_tile = &tiles[i];
        }
    }

    for (size_t i = 0; i < size; i++) {
        if (!tile_is_solved(tiles[i])) {
            remove_all_solved(tiles, size, &tiles[i]);
        }
    }
    solve_if_unique(tiles, size);

    tile_t expected_result = 0;
    for (size_t i = 0; i < strlen(argv[2]); i++) {
        add_to_tile(&expected_result, char_to_tile(argv[2][i]));
    }
    
    return expected_result == *tested_tile ? 0 : 1;
}
