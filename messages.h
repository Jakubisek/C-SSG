#ifndef MESSAGES_H
#define MESSAGES_H

#include <stdio.h>
#include "grid.h"
#include "arg_parser.h"

/*
    Issues that interupt the solving process
    - enabled by default
*/
enum error_msg {
    E_MALOC_FAILED, // %s function name, %s structure
    E_MEMORY_EXCEEDED, // none
    E_SOLUTIONS_EXCEEDED, // none
    E_STACK_INIT_FAILED, // none
    E_INVALID_OPTION, // %s option
    E_INVALID_VALUE, // %s value, %s option
    E_INVALID_INPUT_FILE, // %s file name
};

/*
    Events that can be handled by the program automatically but might require user attention
    - disabled by default
*/
enum warning_msg {
    W_CONFLICTING_SETTINGS, // %s option name
    W_UNNECESSARY_SETTINGS, // %s option name
    W_CANNOT_SET_ZERO_SOLUTIONS, // none
    W_EMPTY_ZERO_EXPANSION, // %ld char position
    W_MISTAKE_IN_INPUT, // none
    W_CHAR_IGNORED, // %c character, %ld postion
    W_INPUT_INCOMPLETE, // %ld added zero count
    W_INPUT_TOO_LONG, // %c ignore char start, %ld ignore pos start
};

/*
    General information about the solving process
    - enabled by default
*/
enum info_msg {
    I_GRID_LOADING_DONE, // %ld numbers count
    I_FILE_LOADING_DONE, // %ld numbers, %s filename
    I_SOLUTION_FOUND, // %ld solution number
    I_SOLVING_DONE, // %ld total solutions
    I_ALL_OK // none
};

void show_error(arg_options_t options, enum error_msg id, size_t argc, ... );


void show_warning(arg_options_t options, enum warning_msg id, size_t argc, ... );


void show_info(arg_options_t options, enum info_msg id, size_t argc, ... );


void debug_msg(arg_options_t options, FILE *log, const char *message, size_t argc, ...);

#endif // MESSAGES_H
