#include "messages.h"

// option default settings:
arg_options_t parsed_options = {
    true, // bool show_errors;
    true, // bool show_warnings;
    true, // bool show_info;

    true, // bool use_color;
    false, // bool compact_solution;
    true, // bool display_solution;

    8, // unsigned long solution_limit;
    NULL, // const char *input_file_name;
    NULL // FILE *debug_out;
};