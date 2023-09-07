#ifndef ARG_PARSER_H
#define ARG_PARSER_H


typedef struct options {
    bool show_errors;
    bool show_warnings;
    bool show_info;
    
    bool use_color;
    bool compact_solution;
    bool display_solution;

    unsigned long solution_limit;
    const char *input_file_name;
    FILE *debug_out;
} arg_options_t;

extern arg_options_t parsed_options;

bool parse_options(int argc, char const *argv[]);

#endif // ARG_PARSER_H