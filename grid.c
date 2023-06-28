#include "grid.h"

tile_t num_to_tile(char num)
{
    if (num == '0') return 0x91FF;
    return (tile_t)(0x1000 + (1 << num & 0xF));
}

tile_t remove_from_tile(tile_t *tile, char num)
{
    if (*tile & (1 << num & 0xF)) return num_to_tile(num);
    return (tile_t)0;
}


int fill_grid(grid_t grid, char const *data)
{
    size_t counter = 0;
    size_t index = 0;
    char c = data[counter];
    while (c != '\0') {
        c = data[counter++];
        if (c <= '/' && c >= ',') continue;
        if (c < '0' || c > '9' || index > 81) return index;
        grid[index++] = num_to_tile(c);
    }
    return (counter == 81);
}

tile_t **get_part(grid_t grid, enum part_type, unsigned long index)
{
    return 0;
}

size_t part_has_tile(grid_t grid, tile_t tile)
{
    return 0;
}

size_t update_all(grid_t grid, unsigned long x, unsigned long y)
{
    return 0;
}