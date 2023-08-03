#include <stdio.h>
#include "tile.h"

tile_t recount(tile_t tile) {
    unsigned char count = 0;
    for (size_t i = 0; i < 9; i++) {
        if (tile & 1) count++;
        tile >>= 1;
    }
    return (count << 12);
}

tile_t char_to_tile(char num)
{
    if (num == '0') return TILE_EMPTY;
    if (num >= '1' && num <= '9') return 0x1000 | (1 << (num - '0' - 1));
    return (tile_t)0;
}

bool tile_is_solved(tile_t tile)
{
    return (tile & 0xF000) == 0x1000;
}

bool tile_has_num(tile_t tile, tile_t num)
{
    return (~(num & 0x0FFF) | (tile & 0x0FFF)) == num; 
}

bool add_to_tile(tile_t *tile, tile_t tile_to_add)
{
    #ifdef DEBUG_MSG
        putchar('\t');
        show_tile(*tile);
        printf(" adding ");
        printf("[%c] ", tile_to_char(tile_to_remove));
        show_tile(tile_to_add);
        putchar('\n');
    #endif
    *tile |= (tile_to_add & 0x0FFF);
    *tile = recount(*tile) | (*tile & 0x0FFF);
    return (*tile & 0x0FFF) ^ (tile_to_add & 0x0FFF); 
}

bool remove_from_tile(tile_t *tile, tile_t tile_to_remove)
{
    #ifdef DEBUG_MSG
        putchar('\t');
        show_tile(*tile);
        printf(" removing ");
        printf("[%c] ", tile_to_char(tile_to_remove));
        show_tile(tile_to_remove);
        putchar('\n');
    #endif
    *tile &= ~(tile_to_remove & 0x0FFF); 
    *tile = recount(*tile) | (*tile & 0x0FFF);
    return (*tile & 0xFFF) & (tile_to_remove & 0xFFF);
}

char tile_to_char(tile_t tile)
{
    if (!(tile & 0xF000)) return '!';
    if (!tile_is_solved(tile)) return '?';
    
    for (size_t i = 0; i < 9; i++) {
        if (tile & 1) return i + '1';
        tile >>= 1;
    }
    return '!';
}

void show_tile(tile_t tile)
{
    putchar(((tile & 0xF000) >> 12) + '0'); putchar('-');
    tile_t mask = 0x100;
    for (size_t i = 0; i < 9; i++) {
        if (tile & mask) {
            putchar('1');
        } else {
            putchar('0');
        }
        mask >>= 1;
    }
}

bool remove_all_solved(tile_t *tiles, size_t count, tile_t *tile_to_update)
{
    if (tile_is_solved(*tile_to_update)) {
        return false;
    }

    tile_t sum = 0;
    for (size_t i = 0; i < count; i++) {
        if (tile_is_solved(tiles[i]) && &tiles[i] != tile_to_update) {
            sum |= tiles[i];
        }
    }
    #ifdef DEBUG_MSG
        putchar('\t');
        show_tile(*tile_to_update);
        printf(" updated with ");
        show_tile(sum);
        putchar('\n');
    #endif
    return remove_from_tile(tile_to_update, sum);
}

bool solve_if_unique(tile_t *tiles, size_t count)
{
    bool result = false;
    unsigned char occured[] = {0,0,0,0,0,0,0,0,0};
    tile_t *last_tile[9];
    for (size_t i = 0; i < count; i++) {
        tile_t tile = tiles[i];
        if (!tile_is_solved(tile)) {
            for (size_t j = 0; j < 9; j++) {
                if (tile & 1) {
                    if (occured[j] == 0) last_tile[j] = &tiles[i];
                    occured[j]++;
                }
                tile >>= 1;
            }
        }
    }
    for (size_t i = 0; i < 9; i++) {
        if (occured[i] == 1) {
            *last_tile[i] = char_to_tile(i + '1');
            result = true;
            #ifdef DEBUG_MSG
                putchar('\t');
                show_tile(*last_tile[i]);
                printf(" contains unique value ");
                putchar(i + '1');
                putchar('\n');
            #endif
        }
    }
    return result;
}
