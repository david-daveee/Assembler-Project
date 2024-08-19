/* Assembler Project (Maman 14) Was created by David Nalivkin tz:332403468*/

#include "prototypes.h"

/*Global variables*/
char externLabels[MAX_SYMBOLS][MAX_LABEL_LENGTH + 1];
int externCount = 0;

/*Handle all entry and create output files*/
void labelEntry() {
    int i, j;
    char binary_str[13]; /* buffer to hold the binary string (12 bits + 1 null terminator) */
    char address_with_suffix[BINARY_CODE_SIZE]; /* buffer to hold the address with "010" suffix */
    char octal_str[6]; /* buffer to hold the octal string (5 digits + 1 null terminator) */
    char padded_address[5]; /* buffer to hold the address padded to 4 digits */
    int found;
    int unresolved_count = 0;
    FILE *unresolved_file;
    char files_name_temp[MAX_FILENAME_LENGTH];
    FILE *output_file;
    int is_binary;
    int binary_value;
    char original_label[MAX_LABEL_LENGTH + 1];

    /* Open the output file for writing */
    sprintf(files_name_temp, "%s.ob", filesName);
    output_file = fopen(files_name_temp, "w");
    if (output_file == NULL) {
        perror("Error opening file");
        return;
    }

    /* Write the IC and DC values as the first line */
    fprintf(output_file, "%d %d\n", IC, DC);

    /* Open the unresolved file for writing if needed */
    unresolved_file = NULL;

    for (i = 0; i < memoryCount; ++i) {
        /* Check if binaryCode contains only '0' and '1' */
        is_binary = 1;
        for (j = 0; j < BINARY_CODE_SIZE - 1; ++j) {
            if (memoryTable[i].binaryCode[j] != '0' && memoryTable[i].binaryCode[j] != '1') {
                is_binary = 0;
                break;
            }
        }
        if (!is_binary) {
            /* If binaryCode is a label */
            found = 0;
            for (j = 0; j < symbolCount; ++j) {
                if (strcmp(memoryTable[i].binaryCode, symbolTable[j].label) == 0) {
                    /* Convert the address to a 12-bit binary string */
                    decimal_to_binary(symbolTable[j].address, binary_str, 13); /* 12 bits + 1 null terminator */

                    /* Add the "010" suffix */
                    strcpy(address_with_suffix, binary_str);
                    strcat(address_with_suffix, "010");

                    /* Replace the binaryCode in the memoryTable with the new binary string */
                    strcpy(memoryTable[i].binaryCode, address_with_suffix);

                    found = 1;
                    break; /* Exit the symbol table loop once we find the label */
                }
            }
            if (!found) {
                /* Label not found, fill binaryCode with "000000000000001" */
                strcpy(original_label, memoryTable[i].binaryCode);
                strcpy(memoryTable[i].binaryCode, "000000000000001");

                /* Open the unresolved file for writing if it's not already open */
                if (unresolved_file == NULL) {
                    sprintf(files_name_temp, "%s.ext", filesName);
                    unresolved_file = fopen(files_name_temp, "w");
                    if (unresolved_file == NULL) {
                        perror("Error opening file");
                        fclose(output_file);
                        return;
                    }
                }

                /* Write the unresolved label and address to the file */
                fprintf(unresolved_file, "%s %d\n", original_label, memoryTable[i].address);
                unresolved_count++;
            }
        }

        /* Convert the address to a 4-digit padded string */
        sprintf(padded_address, "%04d", memoryTable[i].address);
        /* Convert the binary code to an integer */
        binary_value = (int)strtol(memoryTable[i].binaryCode, NULL, 2);

        /* Convert the integer to an octal string */
        decimal_to_octal(binary_value, octal_str, sizeof(octal_str));

        /* Write the padded address and octal code to the output file */
        fprintf(output_file, "%s %s\n", padded_address, octal_str);
    }

    /* Close the output file */
    fclose(output_file);

    /* Close the unresolved file if it was opened */
    if (unresolved_file != NULL) {
        fclose(unresolved_file);
    }
}

/* Function that creates a file entry if needed */
void entryListCreate(const char *filename) {
    FILE *input_file = fopen(filename, "r");
    FILE *entry_file = NULL;
    char line[256];
    char label[MAX_LABEL_LENGTH + 1];
    int i, found;
    int entry_found = 0;
    Entry entries[MAX_SYMBOLS];
    int entry_count = 0;
    int line_number = 0;

    if (input_file == NULL) {
        perror("Error opening input file");
        return;
    }
    while (fgets(line, sizeof(line), input_file)) {
        line_number++;
        if (sscanf(line, ".entry %s", label) == 1) {
            if (isExternLabel(label)) {
                printf("ERROR line: %d Already label exists like external!\n", line_number);
                continue; /* Skip this label if it is already defined as extern */
            }
            found = 0;
            for (i = 0; i < symbolCount; ++i) {
                if (strcmp(label, symbolTable[i].label) == 0) {
                    strcpy(entries[entry_count].label, label);
                    entries[entry_count].address = symbolTable[i].address;
                    entry_count++;
                    found = 1;
                    entry_found = 1;
                    break;
                }
            }

            if (!found) {
                strcpy(entries[entry_count].label, label);
                entries[entry_count].address = 0; /* Optional: handle case if label not found */
                entry_count++;
                entry_found = 1;
            }
        }
    }

    fclose(input_file);

    if (entry_found) {
        char files_name_temp[MAX_FILENAME_LENGTH];
        /* Sort the entries by address */
        sortEntries(entries, entry_count);

        /* Open the entry file for writing */
        sprintf(files_name_temp, "%s.ent", filesName);
        entry_file = fopen(files_name_temp, "w");

        if (entry_file == NULL) {
            perror("Error opening entry file");
            return;
        }

        /* Write the sorted entries to the file */
        for (i = 0; i < entry_count; ++i) {
            fprintf(entry_file, "%s %03d\n", entries[i].label, entries[i].address);
        }

        fclose(entry_file);
    }
}

/* Function to sort entries by address */
void sortEntries(Entry entries[], int count) {
    int i, j;
    Entry temp;

    for (i = 0; i < count - 1; ++i) {
        for (j = 0; j < count - i - 1; ++j) {
            if (entries[j].address > entries[j + 1].address) {
                temp = entries[j];
                entries[j] = entries[j + 1];
                entries[j + 1] = temp;
            }
        }
    }
}

/* Function to check if a label is defined as extern */
int isExternLabel(const char *label) {
    int i;
    for (i = 0; i < externCount; ++i) {
        if (strcmp(label, externLabels[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

/* Running all functions that do what the second pass should do */
void second_pass(const char *filename) {
    labelEntry();
    entryListCreate(filename);
}

