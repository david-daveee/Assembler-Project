/* Assembler Project (Maman 14) Was created by David Nalivkin tz:332403468*/

#ifndef PROTOTYPES_H
#define PROTOTYPES_H
#include "structures.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

/*Global variables */
extern symbolTableEntry *symbolTable;
extern int symbolCount;

extern memoryLine *memoryTable;
extern int memoryCount;

extern int IC;
extern int DC;

extern char filesName [MAX_FILENAME_LENGTH];

extern const char *directivesTypes[4];
extern Command commandsList[16];

extern char externLabels[MAX_SYMBOLS][MAX_LABEL_LENGTH + 1];
extern int externCount;

/*First pass */
void firstPass(const char *filename);
int isDirective(const char* token);
int isCommand(const char* token);
int getOperandType(const char* operand);
void parseOperands(int lineNumber, int number, const char* operands, int operandCount, int* sourceType, int* targetType, char* sourceOperand, char* targetOperand, int* valueOfSourceOperand, int* valueOfTargetOperand, int* flag2registors);
int additionalWords(int sourceOperandType, int targetOperandType);
int calculateAddress(int currentAddress, int additionalWords);
int calculateDirectiveAddress(const char* directiveType, int operandsAmount);
void parseDataOperands(const char* operands, directiveLineFormat *directive);
void parseStringOperands(const char* operands, directiveLineFormat *directive);
void addMemoryLine(memoryLine **memoryTable, int *memoryCount, int address, const char *binaryCode);
void addSymbol(symbolTableEntry **symbolTable, int *symbolCount, const char *label, int address);
void processDirectiveLineFormat(directiveLineFormat *dir, memoryLine **memoryTable, int *memoryCount);
int checkLabel(symbolTableEntry symbolTable[], int symbolCount, char *tempLabel);
int is_first_or_last_comma(const char *str);

/*Assisting functions */
void decimal_to_binary(int num, char *binary_str, size_t str_size);
void decimal_to_octal(int num, char *octal_str, size_t str_size);
char* operandTypeBinary(int number);
void processCommandLineFormat(commandLineFormat *cmd, memoryLine **memoryTable, int *memoryCount);
void freeMemory();
void add_extension_if_needed(char *filename, int flag);
void remove_extension(char *filename);

/*Second pass */
void second_pass(const char *filename);
void labelEntry(void);
void entryListCreate(const char *filename);
void sortEntries(Entry entries[], int count);
int isExternLabel(const char *label);

/*Macro */
void macro(const char *source_filename , const char *after_macro_filename);
int is_command_or_directive(const char *name);
void trim_whitespace(char *str);
void define_macro(const char *name, int line_number);
void add_macro_line(const char *line);
Macro* find_macro(const char *name);
void expand_macro(FILE *outfile, const char *name);
void process_file(const char *source_filename, const char *after_macro);
int check_extra_chars(const char *line);

#endif /* PROTOTYPES_H */

