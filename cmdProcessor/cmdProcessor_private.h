/* 
 * File:   cmdProcessor_private.h
 * Author: Sarp User
 *
 * Created on March 7, 2023, 7:57 PM
 */

#ifndef CMDPROCESSOR_PRIVATE_H
#define	CMDPROCESSOR_PRIVATE_H

#include <stdint.h>
#include <stdio.h>

#include "cmdProcessor.h"
#include "cmdProcessor_config.h"

#define BLANK_ARRAY_PLACEHOLDER 0

#define ERROR_COMMAND_TOO_LONG 0
#define ERROR_TOO_MANY_PARAMS 1
#define ERROR_COMMAND_NOT_FOUND 2

#define PRINT_COMMAND_PROMPT() if (PRINT_CMD_PROMPT_STRING) printf("\n%c ", CMD_PROC_PROMPT_STRING)

#define CMD_PROC_IDLE 0
#define CMD_PROC_PROCESSING 1
#define CMD_PROC_DONE 2
#define CMD_PROC_ERROR 3

typedef struct cmdProcInput_s cmdProcInput_t;

struct cmdProcInput_s{
    char* name;
    char* paramList[MAX_NUM_PARAMS];
    uint8_t numParams;
    volatile uint8_t state;
};

void parseCommand ();
command_t* getCommandPointerFromName (char* name);
void processCommand ();
void clearCommandBuffer ();
command_t* getLastCommandHandle ();
void printErrorMessages (uint8_t error);

#endif	/* CMDPROCESSOR_PRIVATE_H */

