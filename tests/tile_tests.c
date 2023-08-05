#include <stdio.h>
#include "../tile.h"

int add_tile_test(tile_t a, tile_t b, tile_t expected_res, bool expected_ret_val, char const *name)
{
    tile_t old_a = a;
    bool ret_val = add_to_tile(&a, b) == expected_ret_val;
    bool res_tile = expected_res == a;

    printf("[ADD] \033[1;37m%-20s\033[00m ", name);
    show_tile(old_a); putchar(' '); show_tile(b); printf(" = "); show_tile(expected_res);
    printf(" ... %s", (res_tile && ret_val) ? "\033[1;32mTest passed\033[00m" : "\033[1;31mTest failed!\033[00m");
    if (!res_tile) {
        printf("\033[0;31m - actual result: \033[00m");
        show_tile(a);
    }
    if (!ret_val) printf("\033[0;31m - incorrect return value!\033[00m");
    putchar('\n');

    return (ret_val && res_tile) ? 0 : 1;
}

bool remove_tile_test(tile_t a, tile_t b, tile_t expected_res, bool expected_ret_val, char const *name)
{
    tile_t old_a = a;
    bool ret_val = remove_from_tile(&a, b) == expected_ret_val;
    bool res_tile = expected_res == a;

    printf("[SUB] \033[1;37m%-20s\033[00m ", name);
    show_tile(old_a); printf(" - "); show_tile(b); printf(" = "); show_tile(expected_res);
    printf(" ... %s", (res_tile && ret_val) ? "\033[1;32mTest passed\033[00m" : "\033[1;31mTest failed!\033[00m");
    if (!res_tile) {
        printf("\033[0;31m - actual result: \033[00m");
        show_tile(a);
    }
    if (!ret_val) printf("\033[0;31m - incorrect return value!\033[00m");
    putchar('\n');

    return (ret_val && res_tile) ? 0 : 1;
}

int main(int argc, char const *argv[])
{
    int total = 0;
    total += add_tile_test( 0x2101, 0x2005, 0x3105, true, "this is a test");
    total += add_tile_test( 0x3007, 0x3007, 0x3007, true, "another test");


    printf("---\n%sFailed tests: %d\033[00m\n", !total ? "\033[1;34m" : "\033[1;31m", total);
    return total;
}
