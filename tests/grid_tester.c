# include <stdio.h>
# include "../grid.h"

void print_grid(grid_t grid) {
    for (size_t i = 0; i < 81; i++) {
        char c = tile_to_char(grid[i]);
        putchar((c == ' ') ? '.' : c);
    }
    putchar('\n');
}

int main(int argc, char const *argv[])
{
    tile_t grid[81];
    fill_grid(grid, argv[1]);
    while((update_grid(grid) || update_all_unique(grid))) {}
    print_grid(grid);

    if (grid_is_solved(grid) != (argv[2][0] == 's')) return 1;
    if (grid_is_correct(grid) != (argv[2][1] == 'c')) return 1;
    
    return 0;
}