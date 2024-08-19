/* Assembler Project (Maman 14) Was created by David Nalivkin tz:332403468*/

#include "prototypes.h"
/*Directives*/
const char *directivesTypes[4] = {
    ".data", ".string", ".entry", ".extern"
};
/*Commands*/
Command commandsList[16] = {
    {"mov", 2},
    {"cmp", 2},
    {"add", 2},
    {"sub", 2},
    {"lea", 2},
    {"clr", 1},
    {"not", 1},
    {"inc", 1},
    {"dec", 1},
    {"jmp", 1},
    {"bne", 1},
    {"red", 1},
    {"prn", 1},
    {"jsr", 1},
    {"rts", 0},
    {"stop", 0}
};
