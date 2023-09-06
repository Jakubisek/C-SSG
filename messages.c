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

// Add more messages with: {"message", arguments expected in message}
static const message_t get_error_msg[] = {
    {"Malloc failed to allocate reqested memory (%s) in function: '%s'.\n", 2},
    {"The memory reserved for solving stack was exceeded - please report this issue.\n", 0},
    {"Cannot continue solving because the number of found solutions exceeded the given limit.\n", 0},
    {"Solving stack could not be initialized - terminating solving.\n", 0},
    {"Could not recognize option '%s', use --help to see the full list of valid options and syntax.\n", 1},
    {"Value '%s' given for option '%s' was not recognized as valid, use --help to see the correct syntax.\n", 2},
    {"Input file '%s' could not be accessed, either provide a valid file name or define grid input directly.\n", 1}
};
static const message_t get_warning_msg[] = {
    {"Option %s contains conflicting settings to the previous options which will be ignored", 1},
    {"Option %s in unnecessary and will be ignored - removing it will not change the output", 1},
    {"Solution limit was set to 0 which prevents any solutions being displayed. Use --solutions-visibility=hide instead.\n", 0},
    {"Resolved empty zero expansion at %ls, this can be removed since it is unecessary.\n", 1},
    {"Input data contains a trivial mistake resulting in no possible solution,\nre-run with --TODO-ARG-REPLACE-THIS for more information.\n", 0},
    {"Input character '%c' at '%ld' was ignored, resuming loading...\n", 2},
    {"Input is incomplete and was padded with %ld empty spaces, check the input data if this was not intentional.\n", 1},
    {"Input contains too much data - ignoring everything from character '%c' at position %ld.\n", 2}
};
static const message_t get_info_msg[] = {
    {"Loading of the grid was successfully finished after leading %ld numbers", 1},
    {"Finished loading, found %ld numbers in the input file '%s', leading done.\n", 2},
    {"Found and verified solution %ld.\n", 1},
    {"[SOLVING DONE]\nTotal number of solutions found:%ld\n", 1},
    {"No fatal problems were detected encountered during the solving process.\n", 0}
};


void show_error(arg_options_t options, enum error_msg id, size_t argc, ...)
{
    if (!options.show_errors) return;
    if (argc != get_error_msg[id].argc) {
        fprintf(stderr, "!!errargs!!\n");
        return;
    }
    if (options.use_color) fprintf(stderr, "%s", RED);
    fprintf(stderr, "[ERROR]\n");
    if (get_error_msg[id].argc != 0) {
        va_list args;
        va_start(args, argc);
        vfprintf(stderr, get_error_msg[id].message_data, args);
        va_end(args);
    } else fprintf(stderr, get_error_msg[id].message_data);
    
    if (options.use_color) fprintf(stderr, "%s", RESET);
}


void show_warning(arg_options_t options, enum warning_msg id, size_t argc, ...)
{
    if (!options.show_warnings) return;
    if (argc != get_warning_msg[id].argc) {
        fprintf(stderr, "!!warnargs!!\n");
        return;
    }
    if (options.use_color) printf("%s", YELLOW);
    printf("[WARNING]\n");
    if (get_warning_msg[id].argc != 0) {
        va_list args;
        va_start(args, argc);
        vprintf(get_warning_msg[id].message_data, args);
        va_end(args);
    } else printf(get_warning_msg[id].message_data);
    
    if (options.use_color) printf("%s", RESET);
}


void show_info(arg_options_t options, enum info_msg id, size_t argc, ...)
{
    if (!options.show_info) return;
    if (argc != get_info_msg[id].argc) {
        fprintf(stderr, "!!infoargs!!\n");
        return;
    }
    if (get_error_msg[id].argc != 0) {
        va_list args;
        va_start(args, argc);
        vprintf(get_info_msg[id].message_data, args);
        va_end(args);
    } else printf(get_info_msg[id].message_data);
}


void debug_msg(arg_options_t options, FILE *log, const char *message, size_t argc, ...)
{
    if (!options.debug_out) return;
    va_list args;
    va_start(args, argc);
    vfprintf(log, message, args);
    va_end(args);
}