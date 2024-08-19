/* Assembler Project (Maman 14) Was created by David Nalivkin tz:332403468*/

#include "prototypes.h"
char filesName[MAX_FILENAME_LENGTH];

/*Main file that run all proccess for assembler*/
int main(int argc, char *argv[]) {
    char after_macro [MAX_FILENAME_LENGTH];
    char source_filename[MAX_FILENAME_LENGTH];

    if (argc < 2) {
        fprintf(stderr, "Usage: %s <source file>\n", argv[0]);
        return 1;
    }
    
    /* Copy argument to source_filename */
    strncpy(source_filename, argv[1], MAX_FILENAME_LENGTH - 1);
    source_filename[MAX_FILENAME_LENGTH - 1] = '\0';
    /* Add .as, if he is not exist */
    add_extension_if_needed(source_filename, 1);
    /* Add .am  */
    strcpy(after_macro, source_filename);
    remove_extension(after_macro);
    strcpy(filesName, after_macro);
    add_extension_if_needed(after_macro, 2);

    /* Run the macro */
    macro(source_filename, after_macro);

    /* Run the first pass */
    firstPass(after_macro);

    /* Run the second pass */
    second_pass(after_macro);

    /* Free allocated memory */
    freeMemory();

    return 0;
}

