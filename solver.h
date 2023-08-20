#include "grid.h"
#include <stdlib.h>


/**
 * snode_t (struct sudoku_node) represents a node in 9-ary tree, were each node stores the data of its game.
 * 
 * Since any tile can have at most 9 different numbers that could solve it, the children represent all
 * the possible games resulting from solving that tile. Those games than can be solved separately in
 * recursively to find all possible solutions.
 */
typedef struct sudoku_node {
    tile_t game_data[81]; // all the tiles of the solved sudoku
    size_t targeted_pos; // index of the tile that will be solved in children nodes

    size_t depth; // limits the maximum depth of the tree, node cannot have children when this is equal to 0
    size_t children_count; // when equal to 0 this struct can be released - all posibilites were resolved  
    struct sudoku_node *children[9]; // pointers to games with one additional solved tile 
} snode_t;

snode_t *setup_solving_tree(char const *data, size_t initial_depth_limit);

int solve(snode_t *game_to_solve, size_t max_solutions_limit);

void terminate_solving(snode_t *root_node, char const *message);