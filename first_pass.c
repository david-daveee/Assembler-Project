/* Assembler Project (Maman 14) Was created by David Nalivkin tz:332403468*/

#include "prototypes.h"

/*Global variables for handle first pass*/
int IC = 0;
int DC = 0;

symbolTableEntry *symbolTable = NULL;
int symbolCount = 0;

memoryLine *memoryTable = NULL;
int memoryCount = 0;

commandLineFormat *commands = NULL;
int commandCount = 0;
int commandCapacity = 0;

directiveLineFormat *directives = NULL;
int directiveCount = 0;
int directiveCapacity = 0;

/*This function checks if token is a directive*/
int isDirective(const char* token) {
    return strcmp(token, directivesTypes[0]) == 0 || strcmp(token, directivesTypes[1]) == 0 || 
           strcmp(token, directivesTypes[2]) == 0 || strcmp(token, directivesTypes[3]) == 0;
}

/*This function checks if token is a command*/
int isCommand(const char* token) {
    size_t i;
    for (i = 0; i < sizeof(commandsList) / sizeof(commandsList[0]); i++) {
        if (strcmp(token, commandsList[i].name) == 0) {
            return 1;
        }
    }
    return 0;
}

/*Function that recieving operand and return type of the operand*/
int getOperandType(const char* operand) {
    int i;
    if (operand[0] == '#') {
        return 1;
    } else if (operand[0] == 'r') {
        return 4;
    } else if (isalpha(operand[0])) {
        int isValidLabel = 1;
        for (i = 1; operand[i] != '\0'; i++) {
            if (!isalpha(operand[i])) {
                isValidLabel = 1;
                break;
            }
        }
        if (isValidLabel) {
            return 2;
        }
    } else if (strncmp(operand, "*r", 2) == 0) {
        return 3;
    }
    return 0;
}

/*Function that parse opperands and fills the structure and checks for errors*/
void parseOperands(int lineNumber, int number, const char* operands, int operandCount, int* sourceType, int* targetType, char* sourceOperand, char* targetOperand, int* valueOfSourceOperand, int* valueOfTargetOperand, int* flag2registors) {
    char *token = strtok((char *)operands, ", \t\n");
    int count = 0;

    if (operandCount == 0 && !(getOperandType(token) == 0)) {
        printf("ERROR line: %d Not proper amount of operands! \n", lineNumber);
    }

    while (token != NULL && count <= operandCount) {
        if (count < operandCount) {
            if (operandCount == 2 && count == 0) {
                *sourceType = getOperandType(token);
                if (!(*sourceType > 0)) {
                    printf("ERROR line: %d Not proper amount of operands! \n", lineNumber);
                }
                strcpy(sourceOperand, token);
                if (*sourceType == 1) {
                    *valueOfSourceOperand = atoi(token + 1);
                } else if (*sourceType == 3 || *sourceType == 4) {
                    *valueOfSourceOperand = atoi(token + (*sourceType == 3 ? 2 : 1));
                }
            } else {
                *targetType = getOperandType(token);
                if (!(*targetType > 0)) {
                    printf("ERROR line: %d Not proper amount of operands! \n", lineNumber);
                }
                strcpy(targetOperand, token);
                if (*targetType == 1) {
                    *valueOfTargetOperand = atoi(token + 1);
                } else if (*targetType == 3 || *targetType == 4) {
                    *valueOfTargetOperand = atoi(token + (*targetType == 3 ? 2 : 1));
                }
            }
        } else {
            printf("ERROR line: %d Not proper amount of operands! \n", lineNumber);
        }
        if (token == NULL && count == 0 && operandCount == 1) {
            printf("ERROR line: %d Not proper amount of operands! \n", lineNumber);
        }
        token = strtok(NULL, ", \t\n");
        count++;
        if (token == NULL && count == 1 && operandCount == 2) {
            printf("ERROR line: %d Not proper amount of operands! \n", lineNumber);
        }
    }

    if ((*sourceType == 3 || *sourceType == 4) && (*targetType == 3 || *targetType == 4)) {
        *flag2registors = 1;
    } else {
        *flag2registors = 0;
    }

    if (*sourceType == 3 || *sourceType == 4) {
        if (*valueOfSourceOperand > 7 || *valueOfSourceOperand < 0) {
            printf("ERROR line: %d Register not correct! \n", lineNumber);          
        }
    }
    if (*targetType == 3 || *targetType == 4) {
        if (*valueOfTargetOperand > 7 || *valueOfTargetOperand < 0) {
            printf("ERROR line: %d Register not correct! \n", lineNumber);          
        }
    }

    switch (number) {
        case 0:
            if (*targetType == 1) {
                printf("ERROR line: %d Target operand wrong type! \n", lineNumber);
            }
            break;
        case 1:
            break;
        case 2:
            if (*targetType == 1) {
                printf("ERROR line: %d Target operand wrong type! \n", lineNumber);
            }
            break;
        case 3:
            if (*targetType == 1) {
                printf("ERROR line: %d Target operand wrong type! \n", lineNumber);
            }
            break;
        case 4:
            if (*sourceType == 1 || *sourceType == 3 || *sourceType == 4) {
                printf("ERROR line: %d Source operand wrong type! \n", lineNumber);         
            }
            if (*targetType == 1) {
                printf("ERROR line: %d Target operand wrong type! \n", lineNumber);
            }
            break;
        case 5:
            if (*targetType == 1) {
                printf("ERROR line: %d Target operand wrong type! \n", lineNumber);
            }
            break;
        case 6:
            if (*targetType == 1) {
                printf("ERROR line: %d Target operand wrong type! \n", lineNumber);
            }
            break;
        case 7:
            if (*targetType == 1) {
                printf("ERROR line: %d Target operand wrong type! \n", lineNumber);
            }
            break;
        case 8:
            if (*targetType == 1) {
                printf("ERROR line: %d Target operand wrong type! \n", lineNumber);
            }
            break;
        case 9:
            if (*targetType == 1 || *targetType == 4) {
                printf("ERROR line: %d Target operand wrong type! \n", lineNumber);
            }
            break;
        case 10:
            if (*targetType == 1 || *targetType == 4) {
                printf("ERROR line: %d Target operand wrong type! \n", lineNumber);
            }
            break;
        case 11:
            if (*targetType == 1) {
                printf("ERROR line: %d Target operand wrong type! \n", lineNumber);
            }
            break;
        case 12:
            break;
        case 13:
            if (*targetType == 1 || *targetType == 4) {
                printf("ERROR line: %d Target operand wrong type! \n", lineNumber);
            }
            break;
        case 14:
            break;
        case 15:
            break;
        default:
            break;
    }
}

/*Checks how many additional words should with this types of operands*/
int additionalWords(int sourceOperandType, int targetOperandType) {
    if (sourceOperandType == 0 && targetOperandType != 0) {
        return 1;
    } else if (sourceOperandType != 0 && targetOperandType == 0) {
        return 1;
    } else if ((sourceOperandType == 3 || sourceOperandType == 4) && 
               (targetOperandType == 3 || targetOperandType == 4)) {
        return 1;
    } else {
        return 2;
    }
}

/*Calculates address for cmd and count IC*/
int calculateAddress(int currentAddress, int additionalWords) {
    IC = IC + 1 + additionalWords;
    return currentAddress + 1 + additionalWords;
}

/*Calculates addressfor dir and count DC*/
int calculateDirectiveAddress(const char* directiveType, int operandsAmount) {
    if (strcmp(directiveType, ".data") == 0) {
        DC = DC + operandsAmount;
        return operandsAmount;
    } else if (strcmp(directiveType, ".string") == 0) {
        DC = DC + operandsAmount + 1;
        return operandsAmount + 1;
    }
    return 0;
}

/*Parsing operands of .data*/
void parseDataOperands(const char* operands, directiveLineFormat *directive) {
    char *token = strtok((char *)operands, ", \t\n");
    int count = 0;
    while (token != NULL) {
        strcpy(directive->operands[count], token);
        count++;
        token = strtok(NULL, ", \t\n");
    }
    directive->operandCount = count;
}

/*Parsing operands of .string*/
void parseStringOperands(const char* operands, directiveLineFormat *directive) {
    int length = strlen(operands);
    int count = 0;
    int i;
    if (operands[0] == '"' && operands[length - 1] == '"') {
        for (i = 1; i < length - 1; i++) {
            sprintf(directive->operands[count], "%c", operands[i]);
            count++;
        }
    }
    directive->operandCount = count;
}

/*Add memory line to memory table*/
void addMemoryLine(memoryLine **memoryTable, int *memoryCount, int address, const char *binaryCode) {
    memoryLine *temp = (memoryLine *)realloc(*memoryTable, (*memoryCount + 1) * sizeof(memoryLine));
    if (temp == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }
    *memoryTable = temp;

    strncpy((*memoryTable)[*memoryCount].binaryCode, binaryCode, sizeof((*memoryTable)[*memoryCount].binaryCode) - 1);
    (*memoryTable)[*memoryCount].binaryCode[sizeof((*memoryTable)[*memoryCount].binaryCode) - 1] = '\0';
    (*memoryTable)[*memoryCount].address = address;
    if(*memoryCount > MAX_MEMORY_LINES){
	free(commands);
        free(directives);
	free(symbolTable);
	free(memoryTable);
	exit(1);
    }
    (*memoryCount)++;
}

/*Add symbol to symbol table*/
void addSymbol(symbolTableEntry **symbolTable, int *symbolCount, const char *label, int address) {
    symbolTableEntry *temp = (symbolTableEntry *)realloc(*symbolTable, (*symbolCount + 1) * sizeof(symbolTableEntry));
    if (temp == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }
    *symbolTable = temp;

    strncpy((*symbolTable)[*symbolCount].label, label, sizeof((*symbolTable)[*symbolCount].label) - 1);
    (*symbolTable)[*symbolCount].label[sizeof((*symbolTable)[*symbolCount].label) - 1] = '\0';
    (*symbolTable)[*symbolCount].address = address;

    (*symbolCount)++;
}

/*Fills the structure of format of directive*/
void processDirectiveLineFormat(directiveLineFormat *dir, memoryLine **memoryTable, int *memoryCount) {
    int i;
    if (strcmp(dir->type, ".string") == 0) {
        for (i = 0; i < dir->operandCount; i++) {
            char binaryCode[BINARY_CODE_SIZE] = EMPTY_BINARY_CODE;
            decimal_to_binary(dir->operands[i][0], binaryCode, BINARY_CODE_SIZE);
            addMemoryLine(memoryTable, memoryCount, dir->address + i, binaryCode);
        }
        addMemoryLine(memoryTable, memoryCount, dir->address + dir->operandCount, EMPTY_BINARY_CODE);
    } else if (strcmp(dir->type, ".data") == 0) {
        for (i = 0; i < dir->operandCount; i++) {
            char binaryCode[BINARY_CODE_SIZE] = EMPTY_BINARY_CODE;
            decimal_to_binary(atoi(dir->operands[i]), binaryCode, BINARY_CODE_SIZE);
            addMemoryLine(memoryTable, memoryCount, dir->address + i, binaryCode);
        }
    }
}

/*Main function of this file, that run all functions when need and handle first pass*/
void firstPass(const char *filename) {
    FILE *file;
    char line[MAX_LINE_LENGTH + 1];
    int lineNumber = 1;
    int address = 100;
    int i;

    commandCapacity = 10;
    commands = (commandLineFormat *)malloc(commandCapacity * sizeof(commandLineFormat));
    if (commands == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }

    directiveCapacity = 10;
    directives = (directiveLineFormat *)malloc(directiveCapacity * sizeof(directiveLineFormat));
    if (directives == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        free(commands);
        return;
    }

    file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening file %s\n", filename);
        return;
    }

    while (fgets(line, sizeof(line), file)) {

	
        char *trimmed_line;
        size_t len;
        commandLineFormat *cmd;
        directiveLineFormat *dir;
        char *labelEnd;
        line[strcspn(line, "\n")] = '\0';

        trimmed_line = line;

        while (*trimmed_line == ' ' || *trimmed_line == '\t') {
            trimmed_line++;
        }

        len = strlen(trimmed_line);
        while (len > 0 && (trimmed_line[len - 1] == ' ' || trimmed_line[len - 1] == '\t')) {
            trimmed_line[len - 1] = '\0';
            len--;
        }

        if (trimmed_line[0] == '\0' || line[0] == ';') {
            continue;
        }

        if (commandCount >= commandCapacity) {
	    commandLineFormat *temp;
            commandCapacity += 5;
            temp = (commandLineFormat *)realloc(commands, commandCapacity * sizeof(commandLineFormat));
            if (temp == NULL) {
                fprintf(stderr, "Memory allocation failed\n");
                free(commands);
                free(directives);
                fclose(file);
                return;
            }
            commands = temp;
        }

        if (directiveCount >= directiveCapacity) {
	    directiveLineFormat *temp;
            directiveCapacity += 5;
            temp = (directiveLineFormat *)realloc(directives, directiveCapacity * sizeof(directiveLineFormat));
            if (temp == NULL) {
                fprintf(stderr, "Memory allocation failed\n");
                free(commands);
                free(directives);
                fclose(file);
                return;
            }
            directives = temp;
        }

        cmd = &commands[commandCount];
        cmd->address = address;
        cmd->label[0] = '\0';
        cmd->name[0] = '\0';
        cmd->sourceOperand[0] = '\0';
        cmd->targetOperand[0] = '\0';
        cmd->sourceOperandType = 0;
        cmd->targetOperandType = 0;
        cmd->additionalWords = 0;
        cmd->valueOfSourceOperand = 0;
        cmd->valueOfTargetOperand = 0;
        cmd->flag2registors = 0;

        dir = &directives[directiveCount];
        dir->address = address;
        dir->type[0] = '\0';
        dir->operandCount = 0;

        memmove(line, trimmed_line, strlen(trimmed_line) + 1);

        labelEnd = strchr(line, ':');
        if (labelEnd != NULL) {
	    char * nextTokenLableEnd;
            char *token;
	    char *tokenTemp;
            char tempLabel[MAX_LABEL_LENGTH];
            int labelLength = labelEnd - line;
	    char nextToken [MAX_LINE_LENGTH];
            if (labelLength > MAX_LABEL_LENGTH) {
                labelLength = MAX_LABEL_LENGTH;
            }
            strncpy(cmd->label, line, labelLength);
            cmd->label[labelLength] = '\0';

            strncpy(tempLabel, line, labelLength);
            tempLabel[labelLength] = '\0';

	    strncpy(nextToken, line,MAX_LINE_LENGTH);
	    nextTokenLableEnd = strchr(nextToken, ':');
	    tokenTemp = strtok(nextTokenLableEnd + 1, " \t\n");
            
	    if (tokenTemp != NULL && (strcmp(tokenTemp, ".extern") == 0 || strcmp(tokenTemp, ".entry") == 0)){
	    	printf(ERROR" After label can't be .extern or .entry!\n", lineNumber);
	    continue;
            }
	    if (checkLabel(symbolTable, symbolCount, tempLabel)) {
                printf(ERROR" This label already exists! \n", lineNumber);
            } else {
                addSymbol(&symbolTable, &symbolCount, cmd->label, address);
            }

            token = strtok(labelEnd + 1, " \t\n");
            if (token != NULL) {
                if (!isDirective(token) && !isCommand(token)) {
                    printf(ERROR" Command not exists! \n", lineNumber);    
                }
                if (isDirective(token)) {
                    char *operands;
                    strcpy(cmd->name, token);
                    strcpy(dir->type, token);
                    operands = strtok(NULL, "\n");
                    if (strcmp(token, ".data") == 0) {
			if(is_first_or_last_comma(operands)){
				printf(ERROR" Comma not in a proper place!",lineNumber);
			}
                        parseDataOperands(operands, dir);
                    } else if (strcmp(token, ".string") == 0) {
                        parseStringOperands(operands, dir);
                    }
                    address += calculateDirectiveAddress(token, dir->operandCount);
                    directiveCount++;
                } else if (isCommand(token)) {
                    size_t i;
                    char *operands;
                    strcpy(cmd->name, token);
                    for (i = 0; i < sizeof(commandsList) / sizeof(commandsList[0]); i++) {
                        if (strcmp(token, commandsList[i].name) == 0) {
                            cmd->number = i;
                            break;
                        }
                    }
                    operands = strtok(NULL, "\n");
                    if (operands != NULL) {
                        parseOperands(lineNumber, cmd->number, operands, commandsList[cmd->number].operandCount, &cmd->sourceOperandType, &cmd->targetOperandType, cmd->sourceOperand, cmd->targetOperand, &cmd->valueOfSourceOperand, &cmd->valueOfTargetOperand, &cmd->flag2registors);
                        cmd->additionalWords = additionalWords(cmd->sourceOperandType, cmd->targetOperandType);
                    }
                    address = calculateAddress(address, cmd->additionalWords);
                    commandCount++;
                }
            }
        } else {
            char *token = strtok(line, " \t\n");
            if (!isDirective(token) && !isCommand(token)) {
                printf(ERROR" Command not exists! \n", lineNumber);    
            }
            if (token != NULL) {
                if (isDirective(token)) {
                    char *operands;
                    strcpy(cmd->name, token);
                    strcpy(dir->type, token);
                    operands = strtok(NULL, "\n");
                    if (strcmp(token, ".data") == 0) {
                        parseDataOperands(operands, dir);
                    } else if (strcmp(token, ".string") == 0) {
                        parseStringOperands(operands, dir);
                    }
                    address += calculateDirectiveAddress(token, dir->operandCount);
                    directiveCount++;
                } else if (isCommand(token)) {
                    size_t i;
                    char *operands;
                    strcpy(cmd->name, token);
                    for (i = 0; i < sizeof(commandsList) / sizeof(commandsList[0]); i++) {
                        if (strcmp(token, commandsList[i].name) == 0) {
                            cmd->number = i;
                            break;
                        }
                    }
                    operands = strtok(NULL, "\n");
                    if (operands != NULL) {
                        parseOperands(lineNumber, cmd->number, operands, commandsList[cmd->number].operandCount, &cmd->sourceOperandType, &cmd->targetOperandType, cmd->sourceOperand, cmd->targetOperand, &cmd->valueOfSourceOperand, &cmd->valueOfTargetOperand, &cmd->flag2registors);
                        cmd->additionalWords = additionalWords(cmd->sourceOperandType, cmd->targetOperandType);
                    }
                    address = calculateAddress(address, cmd->additionalWords);
                    commandCount++;
                }
            }
        }
        lineNumber++;
    }

    fclose(file);

    for (i = 0; i < commandCount; i++) {
        processCommandLineFormat(&commands[i], &memoryTable, &memoryCount);
    }

    for (i = 0; i < directiveCount; i++) {
        processDirectiveLineFormat(&directives[i], &memoryTable, &memoryCount);
    }

    free(commands);
    free(directives);
}

/*Function thats checks if temp label exists*/
int checkLabel(symbolTableEntry symbolTable[], int symbolCount, char *tempLabel) {
    int i;
    for (i = 0; i < symbolCount; i++) {
        if (strcmp(symbolTable[i].label, tempLabel) == 0) {
            return 1;   
        }
    }
    return 0;
}

/*Checks if comma in a proper place*/
int is_first_or_last_comma(const char *str) {
    const char *start = str;
    const char *end = str + strlen(str) - 1;
    
    while (isspace((unsigned char)*start)) {
        start++;
    }
    
    while (isspace((unsigned char)*end) && end > start) {
        end--;
    }
    
    if (*start == ',' || *end == ',') {
        return 1;
    } else {
        return 0;
    }
}
