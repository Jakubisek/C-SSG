
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


struct options parse_options(int argc, char const *argv[]);

extern arg_options_t parsed_options;