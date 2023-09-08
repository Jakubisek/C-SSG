#include "messages.h"
#include <stdio.h>
#include <stdarg.h>

#define RESET "\033[0m"
#define RED "\033[31m"
#define YELLOW "\033[33m"

typedef struct message_with_arguments {
    const char *message_data;
    int argc;
} message_t;

// Add more messages with: {"message", number of arguments expected in message}
static const message_t get_error_msg[] = {
    {"Malloc failed to allocate reqested memory (%s) in function: '%s'.\n", 2},
    {"The memory reserved for solving stack was exceeded - please report this issue.\n", 0},
    {"Cannot continue solving because the number of found solutions exceeded the given limit.\n", 0},
    {"Solving stack could not be initialized - terminating solving.\n", 0},
    {"Could not recognize option '%s', use --help to see the full list of valid options and syntax.\n", 1},
    {"Value '%s' given for option '%s' was not recognized as valid, use --help to see the correct syntax.\n", 2},
    {"Option '%s' expects a value separated by '=' (example: --argument-name=value), use --help to see the correct syntax.\n", 1},
    {"Input file '%s' could not be accessed, either provide a valid file name or define grid input directly.\n", 1},
    {"Implicit assertion (%s) failed and prevented the program from continuing - please report this issue.\n", 1},
    {"Program had to be abruptly terminated and the solving process could not conclude,\n it is possible that some solutions were skipped.\n", 0},
    {"Debug-log file could not be initialized, no debug messages will be stored.\nTrying to continue anyway...\n", 0},
    {"Cannot solve with no provided arguments, try --help for information about syntax.\n", 0},
    {"Could not parse all input arguments - cannot begin solving process.\n", 0}
};
static const message_t get_warning_msg[] = {
    {"Option '%s' implies conflicting settings to the previous options which will be ignored to resolve this.\n", 1},
    {"Option '%s' in unnecessary and will be ignored - removing it will not change the output.\n", 1},
    {"Solution limit was set to 0 which prevents any solutions being displayed. Use --solutions-visibility=hide instead.\n", 0},
    {"Resolved empty zero expansion at %ld, this can be removed since it is unecessary.\n", 1},
    {"Input data contains a trivial mistake resulting in no possible solution,\nre-run with --TODO-ARG-REPLACE-THIS for more information.\n", 0},
    {"Input character '%c' at '%ld' was ignored, resuming loading...\n", 2},
    {"Input is incomplete and was padded with %ld empty spaces, check the input data if this was not intentional.\n", 1},
    {"Input contains too much data - ignoring everything from character '%c' at position %ld.\n", 2}
};
static const message_t get_info_msg[] = {
    {"Loading of the grid was successfully finished after leading %ld numbers", 1},
    {"Finished loading, found %ld numbers in the input file '%s', leading done.\n", 2},
    {"Found and verified solution %ld (required %ld forks).\n", 2},
    {"SOLVING DONE:\nTotal number of solutions found: %ld\n", 1},
    {"No fatal problems were detected during the solving process - all possible solution were found.\n", 0}
};


void show_error(enum error_msg id, size_t argc, ...)
{
    if (!parsed_options.show_errors) return;
    if (argc != get_error_msg[id - 1].argc) {
        fprintf(stderr, "!!errargs!!\n");
        return;
    }
    if (parsed_options.use_colour) fprintf(stderr, "%s", RED);
    fprintf(stderr, "[ERROR]\n");
    if (get_error_msg[id - 1].argc != 0) {
        va_list args;
        va_start(args, argc);
        vfprintf(stderr, get_error_msg[id - 1].message_data, args);
        va_end(args);
    } else fprintf(stderr, get_error_msg[id - 1].message_data);
    
    if (parsed_options.use_colour) fprintf(stderr, "%s", RESET);
}


void show_warning(enum warning_msg id, size_t argc, ...)
{
    if (!parsed_options.show_warnings) return;
    if (argc != get_warning_msg[id].argc) {
        fprintf(stderr, "!!warnargs!!\n");
        return;
    }
    if (parsed_options.use_colour) printf("%s", YELLOW);
    printf("[WARNING]\n");
    if (get_warning_msg[id].argc != 0) {
        va_list args;
        va_start(args, argc);
        vprintf(get_warning_msg[id].message_data, args);
        va_end(args);
    } else printf(get_warning_msg[id].message_data);
    
    if (parsed_options.use_colour) printf("%s", RESET);
}


void show_info(enum info_msg id, size_t argc, ...)
{
    if (!parsed_options.show_info) return;
    if (argc != get_info_msg[id].argc) {
        fprintf(stderr, "!!infoargs!!\n");
        return;
    }
    if (get_info_msg[id].argc != 0) {
        va_list args;
        va_start(args, argc);
        vprintf(get_info_msg[id].message_data, args);
        va_end(args);
    } else printf(get_info_msg[id].message_data);
}


void debug_msg(const char *message, size_t argc, ...)
{
    if (parsed_options.debug_out == NULL) return;
    va_list args;
    va_start(args, argc);
    vfprintf(parsed_options.debug_out, message, args);
    va_end(args);
    putc('\n', parsed_options.debug_out);
}