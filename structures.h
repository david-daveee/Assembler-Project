/* Assembler Project (Maman 14) Was created by David Nalivkin tz:332403468*/

#ifndef STRUCTURES_H
#define STRUCTURES_H

#define MAX_LABEL_LENGTH 31
#define MAX_LINE_LENGTH 81
#define MAX_OPERANDS 100
#define MAX_SYMBOLS 50
#define MAX_MEMORY_LINES 4096

#define BINARY_CODE_SIZE 16
#define EMPTY_BINARY_CODE "000000000000000"
#define MAX_FILENAME_LENGTH 256

#define MAX_MACRO_LINES 10
#define MAX_MACROS 10

#define ERROR "ERROR line: %d"

typedef struct {
    const char* name;
    int operandCount;
} Command;

typedef struct {
    int address;
    char label[MAX_LABEL_LENGTH + 1];
    int number;
    char name[MAX_LINE_LENGTH + 1];
    char sourceOperand[MAX_LINE_LENGTH + 1];
    char targetOperand[MAX_LINE_LENGTH + 1];
    int sourceOperandType;
    int targetOperandType;
    int valueOfSourceOperand;
    int valueOfTargetOperand;
    int flag2registors;
    int additionalWords;
} commandLineFormat;

typedef struct {
    int address;
    char type[MAX_LINE_LENGTH + 1];
    int operandCount;
    char operands[MAX_OPERANDS][MAX_LINE_LENGTH + 1];
} directiveLineFormat;

typedef struct {
    char name[MAX_LABEL_LENGTH];
    char lines[MAX_MACRO_LINES][MAX_LINE_LENGTH];
    int line_count;
} Macro;

typedef struct {
    int address;
    char label[MAX_LABEL_LENGTH + 1];
} symbolTableEntry;

typedef struct {
    int address;
    char binaryCode[16];
} memoryLine;

typedef struct {
    char label[MAX_LABEL_LENGTH + 1];
    int address;
} Entry;

#endif /* STRUCTURES_H */

