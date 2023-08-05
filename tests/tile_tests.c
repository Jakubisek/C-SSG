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

int main(int argc, char const *argv[])
{
    int i = 0;
    i += add_tile_test( 0x0000, 0x0000, 0x0000, false, "empty + empty");
    i += add_tile_test( 0x91FF, 0x91FF, 0x91FF, false, "full + full");
    i += add_tile_test( 0x0000, 0x91FF, 0x91FF, true , "empty + full");
    i += add_tile_test( 0x91FF, 0x0000, 0x91FF, false, "full + empty");
    i += add_tile_test( 0x1001, 0x1001, 0x1001, false, "1 + 1");
    i += add_tile_test( 0x3007, 0x1001, 0x3007, false, "123 + 1");
    i += add_tile_test( 0x91FF, 0x1010, 0x91FF, false, "full + 5");
    i += add_tile_test( 0x51F0, 0x1001, 0x61F1, true , "56789 + 1");
    i += add_tile_test( 0x400F, 0x40F0, 0x80FF, true , "1234 + 5678");
    i += add_tile_test( 0x60F5, 0x61FA, 0x91FF, true , "94321 alternate");
    i += add_tile_test( 0x40AA, 0x5155, 0x91FF, true , "all alternate 1");
    i += add_tile_test( 0x5155, 0x40AA, 0x91FF, true , "all alternate 2");
    i += add_tile_test( 0x40AA, 0x40AA, 0x40AA, false, "same alternate 1");
    i += add_tile_test( 0x5155, 0x5155, 0x5155, false, "same alternate 2");
    i += add_tile_test( 0x007B, 0x00F9, 0x70FB, true , "random 1");
    i += add_tile_test( 0x0001, 0x012E, 0x612F, true , "random 2");
    i += add_tile_test( 0x01E3, 0x0071, 0x71F3, true , "random 3");
    printf("---\n");
    i += remove_tile_test( 0x0000, 0x0000, 0x0000, false, "empty - empty");
    i += remove_tile_test( 0x91FF, 0x91FF, 0x0000, true , "full - full");
    i += remove_tile_test( 0x0000, 0x91FF, 0x0000, false, "empty - full");
    i += remove_tile_test( 0x91FF, 0x0000, 0x91FF, false, "full - empty");
    i += remove_tile_test( 0x1001, 0x1001, 0x0000, true , "1 - 1");
    i += remove_tile_test( 0x3007, 0x1001, 0x2006, true , "123 - 1");
    i += remove_tile_test( 0x91FF, 0x1010, 0x81EF, true , "full - 5");
    i += remove_tile_test( 0x51F0, 0x1001, 0x51F0, false, "56789 - 1");
    i += remove_tile_test( 0x400F, 0x40F0, 0x400F, false , "1234 - 5678");
    i += remove_tile_test( 0x60F5, 0x61FA, 0x2005, true , "94321 alternate");
    i += remove_tile_test( 0x40AA, 0x5155, 0x40AA, false , "all alternate 1");
    i += remove_tile_test( 0x5155, 0x40AA, 0x5155, false , "all alternate 2");
    i += remove_tile_test( 0x40AA, 0x40AA, 0x0000, true , "same alternate 1");
    i += remove_tile_test( 0x5155, 0x5155, 0x0000, true , "same alternate 2");
    i += remove_tile_test( 0x007B, 0x00F9, 0x1002, true , "random 1");
    i += remove_tile_test( 0x0001, 0x012E, 0x1001, true , "random 2");
    i += remove_tile_test( 0x01E3, 0x0071, 0x3182, true , "random 3");

    printf("---\n%sFailed tests: %d\033[00m\n", !i ? "\033[1;34m" : "\033[1;31m", i);
    return i;
}
