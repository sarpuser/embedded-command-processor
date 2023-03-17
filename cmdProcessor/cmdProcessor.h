/* 
 * File:   cmdProcessor.h
 * Author: Sarp User
 *
 * Created on March 7, 2023, 6:49 PM
 */

#ifndef CMDPROCESSOR_H
#define	CMDPROCESSOR_H

typedef void (*cmdFunction_t) (uint8_t argc, char* argv[]);

typedef struct command_s command_t;

struct command_s {
    char* name;
    cmdFunction_t cmdFunction;
    char* description;
    char* paramInfo;
    command_t* prevCommandHandle;
};

void CMD_PROCESSOR_Init ();
void CMD_PROCESSOR_RegisterCommand (command_t* cmdToRegister);
void CMD_PROCESSOR_Tasks ();
void CMD_PROCESSOR_AddCharToBuffer (char charToAdd);

#endif	/* CMDPROCESSOR_H */

