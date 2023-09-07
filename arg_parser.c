#include "messages.h"
#include "arg_parser.h"


// change the default settings here:
arg_options_t parsed_options = {
    true, // bool show_errors
    true, // bool show_warnings
    true, // bool show_info

    true,  // bool use_colour
    false, // bool compact_solution
    true,  // bool display_solution

    3,    // unsigned long solution_limit
    NULL, // const char *input_file_name
    NULL  // FILE *debug_out
};

#define DEBUG_LOG_FILE_NAME "cssg_debug_log.txt"


static bool parse_ulong(char const *str_to_parse, size_t *parse_result)
{
    size_t char_counter = 0;
    char c;
    while ((c = str_to_parse[char_counter++]) != '\0') {
        if (c > '9' || c < '0') return false;
        *parse_result = (*parse_result * 10) + c - '0';
    }
    return true;
}


static const char *separate_value(char const *str_to_split)
{
    size_t char_counter = 0;
    char c;
    while ((c = str_to_split[char_counter++]) != '\0') {
        if (c == '=') {
            return &(str_to_split[char_counter]);
        }
    }
    return NULL;
}


static bool str_equal(char const *str1, char const *str2)
{
    size_t char_counter = 0;
    while (str1[char_counter] != '\0' && str2[char_counter] != '\0') {
        if (str1[char_counter] != str2[char_counter]) return false;
        char_counter++;
    }
    // ingnore value
    if (str1[char_counter] == '=' || str2[char_counter] == '=') return true;
    return str1[char_counter] == str2[char_counter];
}

struct changed_options {
    bool sol_limit_set;
    bool colour_set;
    bool sol_visibility_set;
    bool message_visibility_set;
    bool file_input_set;
    bool debug_log_used;
};


static bool single_option_parse(char const *option_string, struct changed_options *changes, int argc, char const *argv[])
{
    char const *value = separate_value(option_string);
    char const *option = option_string;

    // CHANGE THE LIMIT OF THE MAXIMUM SOLUTION THAT SHOULD BE DISPLAYED
    if (str_equal(option, "--solution-limit") || str_equal(option, "-sl")) {
        
        // requires value
        if (value == NULL) {
            show_error(E_MISSING_VALUE, 1, option_string);
            return false;
        }
        // value must be numeric (size_t)
        size_t numeric_value;
        if (!parse_ulong(value, &numeric_value)) {
            show_error(E_INVALID_VALUE, 2, value, option_string);
            return false;
        }

        // check if unnecessary
        if (parsed_options.solution_limit == numeric_value) {
            show_warning(W_UNNECESSARY_SETTINGS, 1, option_string);
        }

        // cannot be reused again
        if (changes->sol_limit_set) {
            show_warning(W_CONFLICTING_SETTINGS, 1, option_string);
        }

        // warn when 0
        if (numeric_value == 0) {
            show_warning(W_CANNOT_SET_ZERO_SOLUTIONS, 0);
        }

        changes->sol_limit_set = true;
        parsed_options.solution_limit = numeric_value;
        return true;
    }

    // TOGGLE IF COLOURS SHOULD BE USED TO DISPLAY MESSAGES
    if (str_equal(option, "--use-colours") || str_equal(option, "-uc")) {
        if (changes->colour_set || parsed_options.use_colour || changes->message_visibility_set) {
            show_warning(W_UNNECESSARY_SETTINGS, 1, option);
        }
        parsed_options.use_colour = true;
        changes->colour_set = true;
        return true;
    }
    if (str_equal(option, "--no-colours") || str_equal(option, "-nc")) {
        if (changes->colour_set || !parsed_options.use_colour) {
            show_warning(W_UNNECESSARY_SETTINGS, 1, option);
        }
        parsed_options.use_colour = false;
        changes->colour_set = true;
        return true;
    }

    // LIMIT THE NUMBER OF TEXT DISPLAYED
    if (str_equal(option, "--minimal-text") || str_equal(option, "-mt")) {
        // no reminder for user preferences since they would have to be altered
        if (changes->message_visibility_set) {
            show_warning(W_UNNECESSARY_SETTINGS, 1, option);
        }

        // change --minimal-text preset here:
        parsed_options.show_errors = true; // errors should always be enabled
        parsed_options.show_warnings= false;
        parsed_options.show_info = false;
        parsed_options.compact_solution = true;
        changes->message_visibility_set= true;
        return true;
    }

    // CREATE DEBUG-LOG FILE
    if (str_equal(option, "--create-debug-log") || str_equal(option, "-dl")) {
        if (changes->debug_log_used || parsed_options.debug_out == NULL) {
            show_warning(W_UNNECESSARY_SETTINGS, 1, option);
        }
        FILE *dlog = fopen(DEBUG_LOG_FILE_NAME, "w");
        if (dlog == NULL) {
            show_error(E_DEBUG_LOG_INIT_FAILED, 0);
            return false;
        }
        parsed_options.debug_out = dlog;
        return true;
    }

    // USE FILE AS INPUT FILE
    if (str_equal(option, "--file-input") || str_equal(option, "-fi")) {
        if (changes->file_input_set) {
            show_warning(W_UNNECESSARY_SETTINGS, 1, option);
            return false;
        }
        parsed_options.input_file_name = argv[argc - 1];
        return true;
    }
    
    // CHANGE SOLUTION VISIBILITY
    if (str_equal(option, "--solution-visibility") || str_equal(option, "-sv")) {
        
        // requires value
        if (value == NULL) {
            show_error(E_MISSING_VALUE, 1, option);
            return false;
        }
        
        if (!str_equal(value, "hide") && !str_equal(value, "show") && !str_equal(value, "compact")){
            show_error(E_INVALID_VALUE, 2, value, option);
            return false;
        }

        if (changes->sol_visibility_set) {
            show_warning(W_UNNECESSARY_SETTINGS, 1, option_string);
            return false;
        }
        if (str_equal(value, "hide")) {
            if (!parsed_options.display_solution) show_warning(W_UNNECESSARY_SETTINGS, 1, option_string);
            parsed_options.display_solution = false;
        } else if (str_equal(value, "show")) {
            if (parsed_options.display_solution) show_warning(W_UNNECESSARY_SETTINGS, 1, option_string);
            parsed_options.display_solution = true;
        } else {
            if (parsed_options.compact_solution) show_warning(W_UNNECESSARY_SETTINGS, 1, option_string);
            parsed_options.compact_solution = true;
        }
        return true;
    }
    
    show_error(E_INVALID_OPTION, 1, option);
    return false;
}


bool parse_options(int argc, char const *argv[]) {
    if (argc <= 1) {
        show_error(E_NO_ARGUMENTS, 0);
        return false;
    }
    struct changed_options changes = {false, false, false, false, false, false};
    for (size_t i = 1; i < argc - 1; i++) {
        if (!single_option_parse(argv[i], &changes, argc, argv)) return false;
    }
    return true;
}