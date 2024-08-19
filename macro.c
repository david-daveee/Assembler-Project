/* Assembler Project (Maman 14) Was created by David Nalivkin tz:332403468*/

#include "prototypes.h"

Macro macros[MAX_MACROS];
int macro_count = 0;

/*The function is_command_or_directive checks whether a given name (a string) is either a command or a directive.*/
int is_command_or_directive(const char *name) {
    size_t i;
    for (i = 0; i < sizeof(commandsList) / sizeof(commandsList[0]); i++) {
        if (strcmp(name, commandsList[i].name) == 0) {
            return 1;
        }
    }
    for (i = 0; i < sizeof(directivesTypes) / sizeof(directivesTypes[0]); i++) {
        if (strcmp(name, directivesTypes[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

/*The trim_whitespace function removes leading and trailing whitespace from a given string.*/
void trim_whitespace(char *str) {
    char *end;
    while (isspace((unsigned char)*str)) str++; /* Trim leading space */
    if (*str == 0) return; /* All spaces */

    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--; /* Trim trailing space */

    end[1] = '\0';
}

/*The define_macro function is designed to define a new macro in a program. And checks if name is proper of macro*/
void define_macro(const char *name, int line_number) {
	char trimmed_name[MAX_LINE_LENGTH];
    if (macro_count >= MAX_MACROS) {
        fprintf(stderr, "Error: too many macros defined.\n");
        exit(1);
    }

    strcpy(trimmed_name, name);
    trim_whitespace(trimmed_name);

    if (is_command_or_directive(trimmed_name)) {
        fprintf(stderr, ERROR" Macro name is not proper!\n", line_number);
        exit(1);
    }
    strcpy(macros[macro_count].name, trimmed_name);
    macros[macro_count].line_count = 0;
    macro_count++;
}

/*The add_macro_line function adds a line of code to the most recently defined macro,
 ensuring it doesn't exceed the maximum allowed lines, and exits with an error message if it does.*/
void add_macro_line(const char *line) {
    if (macro_count > 0) {
        Macro *current_macro = &macros[macro_count - 1];
        if (current_macro->line_count >= MAX_MACRO_LINES) {
            fprintf(stderr, "Error: too many lines in macro %s.\n", current_macro->name);
            exit(1);
        }
        strcpy(current_macro->lines[current_macro->line_count], line);
        current_macro->line_count++;
    }
}

/*The find_macro function searches for a macro by name, 
after trimming any whitespace, and returns a pointer to the macro if found, or NULL if not.*/
Macro* find_macro(const char *name) {
    int i;
    char trimmed_name[MAX_LINE_LENGTH];
    strcpy(trimmed_name, name);
    trim_whitespace(trimmed_name);

    for (i = 0; i < macro_count; i++) {
        if (strcmp(macros[i].name, trimmed_name) == 0) {
            return &macros[i];
        }
    }
    return NULL;
}

/*The expand_macro function writes the lines of a specified macro to an output file,
 or prints an error message if the macro is not found.*/
void expand_macro(FILE *outfile, const char *name) {
    Macro *macro = find_macro(name);
    if (macro != NULL) {
        int i;
        for (i = 0; i < macro->line_count; i++) {
            fprintf(outfile, "%s\n", macro->lines[i]);
        }
    } else {
        fprintf(stderr, "Error: macro %s not found.\n", name);
    }
}

/*The process_file function reads a source file line by line, processes macros by defining and expanding them,
 and writes the resulting output to a new file, handling any errors appropriately.*/
void process_file(const char *source_filename, const char *after_macro) {
    char line[MAX_LINE_LENGTH];
    int in_macro_definition = 0;
    int line_number = 0;
    FILE *infile = fopen(source_filename, "r");
    FILE *outfile = fopen(after_macro, "w");

    if (infile == NULL) {
        fprintf(stderr, "Error opening source file %s.\n", source_filename);
        exit(1);
    }
    if (outfile == NULL) {
        fprintf(stderr, "Error opening temporary file %s.\n", after_macro);
        fclose(infile);
        exit(1);
    }



    while (fgets(line, sizeof(line), infile) != NULL) {
	char *trimmed_line;
        line_number++;

	if (strlen(line) > MAX_LINE_LENGTH - 2) {
            fprintf(stderr, ERROR" exceeds maximum length of %d characters.\n", line_number, MAX_LINE_LENGTH - 1);
        }

        line[strcspn(line, "\n")] = '\0';  /* Remove newline character */

        /* Skip leading spaces and tabs */
        trimmed_line = line + strspn(line, " \t");

        /* Check for empty line after removing spaces */
        if (trimmed_line[0] == '\0') {
            continue;
        }

        if (strncmp(trimmed_line, "macr ", 5) == 0) {
            char *macro_name = trimmed_line + 5;
	    char *name_end;
            macro_name += strspn(macro_name, " \t");  /* Skip spaces after "macr" */
	    name_end = macro_name + strcspn(macro_name, " \t");
    	    if (check_extra_chars(name_end)) {
        	    fprintf(stderr, ERROR" Macro name is not proper!\n", line_number);
        	    exit(1);
    	    }

            define_macro(macro_name, line_number);
            in_macro_definition = 1;
        } else if (strncmp(trimmed_line, "endmacr", 7) == 0) {
    		char *endmacr_end;
    		endmacr_end = trimmed_line + 7;
    		if (check_extra_chars(endmacr_end)) {
        		fprintf(stderr, ERROR" 'endmacr' has extra characters!\n", line_number);
        		exit(1);
    		}
    	in_macro_definition = 0;
        } else if (in_macro_definition) {
            add_macro_line(trimmed_line);
        } else {
            Macro *macro = find_macro(trimmed_line);
            if (macro != NULL) {
                expand_macro(outfile, trimmed_line);
            } else {
                fprintf(outfile, "%s\n", trimmed_line);
            }
        }
    }

    fclose(infile);
    fclose(outfile);
}

/*The check_extra_chars function checks if there are any non-whitespace characters remaining in a string after a certain point.*/
int check_extra_chars(const char *line) {
    /* Check if the line contains only spaces after the macro keyword or endmacr */
    while (*line != '\0') {
        if (!isspace((unsigned char)*line)) {
            return 1;
        }
        line++;
    }
    return 0;
}

/*Handle pre processor pass*/
void macro(const char *source_filename , const char *after_macro_filename) {
    process_file(source_filename, after_macro_filename);
}

