#ifndef MESSAGES_H
#define MESSAGES_H

#include <stdio.h>
#include "grid.h"
#include "arg_parser.h"

/*
    Issues that interupt the solving process
    - should be enabled by default
*/
enum error_msg {
    E_MALOC_FAILED = 1, // %s function name, %s structure
    E_MEMORY_EXCEEDED, // none
    E_SOLUTIONS_EXCEEDED, // none
    E_STACK_INIT_FAILED, // none
    E_INVALID_OPTION, // %s option
    E_INVALID_VALUE, // %s value, %s option
    E_MISSING_VALUE, // %s option
    E_INVALID_INPUT_FILE, // %s file name
    E_IMPLICIT_ASSERTION_FAILED, // %s where
    E_ABRUPT_TERMINATION_MSG, // none
    E_DEBUG_LOG_INIT_FAILED, // none
    E_NO_ARGUMENTS, // none
    E_ARGUMENT_PARSING_FAILED // none
};

/*
    Events that can be handled by the program automatically but might require user attention
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
*/
enum info_msg {
    I_GRID_LOADING_DONE, // %ld numbers count
    I_FILE_LOADING_DONE, // %ld numbers, %s filename
    I_SOLUTION_FOUND, // %ld solution number, %ld depth
    I_SOLVING_DONE, // %ld total solutions
    I_ALL_OK // none
};

void show_error(enum error_msg id, size_t argc, ... );


void show_warning(enum warning_msg id, size_t argc, ... );


void show_info(enum info_msg id, size_t argc, ... );


void debug_msg(const char *message, size_t argc, ...);

#endif // MESSAGES_H
