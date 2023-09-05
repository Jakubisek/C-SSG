#ifndef MESSAGES_H
#define MESSAGES_H

#include "grid.h"

/*
    Issues that interupt the solving process
    - enabled by default
*/
enum error_msg {
    MSG1,
    MSG2
};


/*
    Events that can be handled by the program automatically but might require user attention
    - disabled by default
*/
enum warning_msg {
    MSG1,
    MSG2
};


/*
    General information about the solving process
    - enabled by default
*/
enum info_msg {
    MSG1,
    MSG2
};

/*
    Additional information about the solving process and specific perfomed operation
    - disabled by default
*/
enum debug_msg {
    MSG1,
    MSG2
};

// TODO
// OPTIONS SHOULD BE RPLACED BY STRUCT POINTER WHEN ARGUMENT PARSING IS DONE

void show_error(int options, enum error_msg id, ... );


void show_warning(int options, enum warning_msg id, ... );


void show_info(int options, enum info_msg id, ... );


void show_tile(int options, tile_t tile);


void show_grid(int options, grid_t grid);


#endif // MESSAGES_H
