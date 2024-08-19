/* Assembler Project (Maman 14) Was created by David Nalivkin tz:332403468*/

#include "prototypes.h"

/*Convert decimal to binary in format that you need of amount of bits*/
void decimal_to_binary(int num, char *binary_str, size_t str_size) {
    unsigned int mask = 1;
    int index;
    
    memset(binary_str, '0', str_size - 1);
    binary_str[str_size - 1] = '\0';

    if (num < 0) {
        num = (~(-num) + 1) & ((1 << (str_size - 1)) - 1); 
    }

    index = str_size - 2;

    while (index >= 0) {
        binary_str[index--] = (num & mask) ? '1' : '0';
        num >>= 1;
    }
}

/*Convert decimal to octal in format that you need of amount of bits*/
void decimal_to_octal(int num, char *octal_str, size_t str_size) {
    int index;

    memset(octal_str, '0', str_size - 1);
    octal_str[str_size - 1] = '\0';

    if (num < 0) {
        num = -num;
    }

    index = str_size - 2;
    while (num > 0 && index >= 0) {
        octal_str[index--] = (num % 8) + '0';
        num /= 8;
    }
}

/*Return types of operands in bit format*/
char* operandTypeBinary(int number) {
    switch (number) {
        case 1:
            return "0001";
        case 2:
            return "0010";
        case 3:
            return "0100";
        case 4:
            return "1000";
        default:
            return "0000";
    }
}

/*Convert proparly structures and data to memory table*/
void processCommandLineFormat(commandLineFormat *cmd, memoryLine **memoryTable, int *memoryCount) {
    char binaryCode[16] = EMPTY_BINARY_CODE;
    char opcodeBin[5];
    char sourceTypeBin[5];
    char targetTypeBin[5];
    int i;

    decimal_to_binary(cmd->number, opcodeBin, sizeof(opcodeBin));
    strcpy(sourceTypeBin, operandTypeBinary(cmd->sourceOperandType));
    strcpy(targetTypeBin, operandTypeBinary(cmd->targetOperandType));

    for (i = 0; i < 4; i++) {
        binaryCode[i] = opcodeBin[i];
        binaryCode[i + 4] = sourceTypeBin[i];
        binaryCode[i + 8] = targetTypeBin[i];
    }
    binaryCode[12] = '1';
    binaryCode[13] = '0';
    binaryCode[14] = '0';

    addMemoryLine(memoryTable, memoryCount, cmd->address, binaryCode);

    if (cmd->flag2registors == 1) {
        char additionalWord[16] = EMPTY_BINARY_CODE;
        char sourceRegBin[4];
        char targetRegBin[4];

        decimal_to_binary(cmd->valueOfSourceOperand, sourceRegBin, sizeof(sourceRegBin));
        decimal_to_binary(cmd->valueOfTargetOperand, targetRegBin, sizeof(targetRegBin));

        for (i = 0; i < 3; i++) {
            additionalWord[i + 6] = sourceRegBin[i];
            additionalWord[i + 9] = targetRegBin[i];
        }
        additionalWord[12] = '1';
        additionalWord[13] = '0';
        additionalWord[14] = '0';

        addMemoryLine(memoryTable, memoryCount, cmd->address + 1, additionalWord);
    }

    if (cmd->additionalWords == 2) {
        char additionalWord1[BINARY_CODE_SIZE] = EMPTY_BINARY_CODE;
        char additionalWord2[BINARY_CODE_SIZE] = EMPTY_BINARY_CODE;

        if (cmd->sourceOperandType == 1) {
            char sourceOperandBin[13];
            decimal_to_binary(cmd->valueOfSourceOperand, sourceOperandBin, sizeof(sourceOperandBin));
            for (i = 0; i < 12; i++) {
                additionalWord1[i] = sourceOperandBin[i];
            }
            additionalWord1[12] = '1';
            additionalWord1[13] = '0';
            additionalWord1[14] = '0';
        } else if (cmd->sourceOperandType == 2) {
            for (i = 0; i < 14; i++) {
                additionalWord1[i] = cmd->sourceOperand[i];
            }
        } else if (cmd->sourceOperandType == 3 || cmd->sourceOperandType == 4) {
            char sourceRegBin[4];
            decimal_to_binary(cmd->valueOfSourceOperand, sourceRegBin, sizeof(sourceRegBin));
            for (i = 0; i < 3; i++) {
                additionalWord1[i + 6] = sourceRegBin[i];
            }
            additionalWord1[12] = '1';
            additionalWord1[13] = '0';
            additionalWord1[14] = '0';
        }

        if (cmd->targetOperandType == 1) {
            char targetOperandBin[13];
            decimal_to_binary(cmd->valueOfTargetOperand, targetOperandBin, sizeof(targetOperandBin));
            for (i = 0; i < 12; i++) {
                additionalWord2[i] = targetOperandBin[i];
            }
            additionalWord2[12] = '1';
            additionalWord2[13] = '0';
            additionalWord2[14] = '0';
        } else if (cmd->targetOperandType == 2) {
            for (i = 0; i < 14; i++) {
                additionalWord2[i] = cmd->targetOperand[i];
            }
        } else if (cmd->targetOperandType == 3 || cmd->targetOperandType == 4) {
            char targetRegBin[4];
            decimal_to_binary(cmd->valueOfTargetOperand, targetRegBin, sizeof(targetRegBin));
            for (i = 0; i < 3; i++) {
                additionalWord2[i + 9] = targetRegBin[i];
            }
            additionalWord2[12] = '1';
            additionalWord2[13] = '0';
            additionalWord2[14] = '0';
        }

        addMemoryLine(memoryTable, memoryCount, cmd->address + 1, additionalWord1);
        addMemoryLine(memoryTable, memoryCount, cmd->address + 2, additionalWord2);
    } else if (cmd->additionalWords == 1 && cmd->flag2registors == 0) {
        char additionalWord[BINARY_CODE_SIZE] = EMPTY_BINARY_CODE;

        if (cmd->targetOperandType == 1) {
            char targetOperandBin[13];
            decimal_to_binary(cmd->valueOfTargetOperand, targetOperandBin, sizeof(targetOperandBin));
            for (i = 0; i < 12; i++) {
                additionalWord[i] = targetOperandBin[i];
            }
            additionalWord[12] = '1';
            additionalWord[13] = '0';
            additionalWord[14] = '0';
        } else if (cmd->targetOperandType == 2) {
            for (i = 0; i < 14; i++) {
                additionalWord[i] = cmd->targetOperand[i];
            }
        } else if (cmd->targetOperandType == 3 || cmd->targetOperandType == 4) {
            char targetRegBin[4];
            decimal_to_binary(cmd->valueOfTargetOperand, targetRegBin, sizeof(targetRegBin));
            for (i = 0; i < 3; i++) {
                additionalWord[i + 9] = targetRegBin[i];
            }
            additionalWord[12] = '1';
            additionalWord[13] = '0';
            additionalWord[14] = '0';
        }

        addMemoryLine(memoryTable, memoryCount, cmd->address + 1, additionalWord);
    }
}

/*Free memory of global dinamic variables*/
void freeMemory() {
    if (memoryTable != NULL) {
        free(memoryTable);
        memoryTable = NULL;
    }
    if (symbolTable != NULL) {
        free(symbolTable);
        symbolTable = NULL;
    }
}

/*The add_extension_if_needed function appends ".as" or ".am" to the given filename if the specified flag is 1 or 2,
 respectively, and the filename does not already have that extension.*/
void add_extension_if_needed(char *filename, int flag) {
    if (flag == 1) {
        if (strstr(filename, ".as") == NULL) {
            strncat(filename, ".as", MAX_FILENAME_LENGTH - strlen(filename) - 1);
        }
    }
    if (flag == 2) {
        if (strstr(filename, ".am") == NULL) {
            strncat(filename, ".am", MAX_FILENAME_LENGTH - strlen(filename) - 1);
        }
    }
}

/*The remove_extension function removes the file extension from 
a given filename by truncating the string at the last dot ('.') character if one exists.*/
void remove_extension(char *filename) {
    char *dot;
    dot = strrchr(filename, '.');
    if (dot != NULL) {
        *dot = '\0';
    }
}

