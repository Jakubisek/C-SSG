#include <stdio.h>
#include "grid.h"

tile_t num_to_tile(char num)
{
    if (num == '0') return 0x91FF;
    return (tile_t)(0x1000 | (1 << (num - '1')));
}

tile_t remove_from_tile(tile_t *tile, char num)
{
    if (*tile & (1 << (num - '0'))) return num_to_tile(num);
    return (tile_t)0;
}


int fill_grid(grid_t grid, char const *data)
{
    size_t counter = 0;
    size_t index = 0;
    char c = data[counter];
    while (c != '\0') {
        c = data[counter++];
        if ((c <= '/' && c >= ',') || c == ' ') continue;
        if (index >= 81 && c != '\0') {
            fprintf(stderr, "Input contains redundatnt data that will be ignored\n");
            return 82;
        }
        if (c < '0' || c > '9') return index;
        grid[index++] = num_to_tile(c);
    }
    return counter;
}

void get_part(grid_t grid, tile_t **part, enum part_type, unsigned long index)
{

}

size_t part_has_tile(grid_t grid, tile_t tile)
{
    return 0;
}

size_t update_all(grid_t grid, unsigned long x, unsigned long)
{
    return 0;
}

char tile_as_char(tile_t tile) {
    if (tile > 0x1100) return '?';
    for (size_t i = 0; i < 9; i++) {
        if (tile & 1) return '1' + i;
        tile >>= 1;
    }
    return '!';
}

void show_grid(grid_t grid) {
    printf("+---+---+---+ +---+---+---+ +---+---+---+\n");
    for (size_t i = 0; i < 9; i++) {
        for (size_t j = 0; j < 9; j++) {
            printf("| %c ", tile_as_char(grid[9*i+j]));
            if (j % 3 == 2 && j != 8) printf("| ");
        }
        printf("|\n+---+---+---+ +---+---+---+ +---+---+---+\n");
        if (i % 3 == 2 && i != 8) printf("+---+---+---+ +---+---+---+ +---+---+---+\n");
    }
    putchar('\n');
}