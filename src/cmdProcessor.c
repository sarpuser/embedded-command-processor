#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#include "cmdProcessor.h"
#include "cmdProcessor_private.h"
#include "cmdProcessor_config.h"
#include "cmdProcessor_builtincommands.h"
#include "commands/commands.h"

static cmdProcInput_t cmdProcInput;

static command_t* lastCommandHandle = NULL;

static char cmd[MAX_COMMAND_SIZE] = {BLANK_ARRAY_PLACEHOLDER};
static char circularBuffer[CIRCULAR_BUFFER_SIZE] = {BLANK_ARRAY_PLACEHOLDER};

static char* circularBufferEnd = circularBuffer + CIRCULAR_BUFFER_SIZE - 1;
static char* circularBufferWritePointer = circularBuffer;
static char* circularBufferReadPointer = circularBuffer;

void CMD_PROCESSOR_Init () {
    cmdProcInput.state = CMD_PROC_IDLE;
    registerBuiltinCommands();
    registerCustomCommands();
    PRINT_COMMAND_PROMPT();
}

void CMD_PROCESSOR_RegisterCommand (command_t* cmdToRegister) {
    cmdToRegister->prevCommandHandle = lastCommandHandle;
    lastCommandHandle = cmdToRegister;
}

void CMD_PROCESSOR_AddCharToBuffer (char charToAdd) {
    *circularBufferWritePointer = charToAdd;
    
    if (circularBufferWritePointer != circularBufferEnd) circularBufferWritePointer++;
    else circularBufferWritePointer = circularBuffer;
}

void CMD_PROCESSOR_Tasks () {
    uint8_t i = 0;
    while (circularBufferReadPointer != circularBufferWritePointer && cmdProcInput.state == CMD_PROC_IDLE) {
        if (i < MAX_COMMAND_SIZE) cmd[i++] = *circularBufferReadPointer;
        else {
            printErrorMessages(ERROR_COMMAND_TOO_LONG);
            circularBufferReadPointer = circularBufferWritePointer;
            break;
        } 
       
        if (circularBufferReadPointer != circularBufferEnd) circularBufferReadPointer++;
        else circularBufferReadPointer = circularBuffer;
    }
    
    if (cmd[0] != BLANK_ARRAY_PLACEHOLDER && cmdProcInput.state != CMD_PROC_ERROR) {
        cmdProcInput.state = CMD_PROC_PROCESSING;
        parseCommand();
        
        if (cmdProcInput.state != CMD_PROC_ERROR) {
            getCommandPointerFromName(cmdProcInput.name);
            processCommand();
        }
    }
    
    if (cmdProcInput.state == CMD_PROC_ERROR || cmdProcInput.state == CMD_PROC_DONE) {
        clearCommandBuffer();
        PRINT_COMMAND_PROMPT();
    }
}

void parseCommand () {
    uint8_t i = 0;
    uint8_t paramIndex = 0;
    uint8_t wordBeginningIndex = 0;
    bool firstWordParsed = false;
    while (cmd[wordBeginningIndex] != BLANK_ARRAY_PLACEHOLDER) {   
        if (cmd[i] == ' ' || cmd[i] == '\n' || cmd[i] =='\r') {
            cmd[i] = '\0';
            if (cmd[wordBeginningIndex] != '\0') { // This is used to ignore extra spaces/newline characters
                if (!firstWordParsed) {
                    firstWordParsed = true;
                    cmdProcInput.name = &cmd[wordBeginningIndex];
                }
                else if (cmdProcInput.numParams < MAX_NUM_PARAMS) {
                    cmdProcInput.paramList[paramIndex++] = &cmd[wordBeginningIndex];
                    cmdProcInput.numParams = paramIndex;
                }
                else printErrorMessages(ERROR_TOO_MANY_PARAMS);
            }
            wordBeginningIndex = i + 1;
        }
        i++;
    }
    
    if (!firstWordParsed) cmdProcInput.state = CMD_PROC_ERROR;
}

command_t* getCommandPointerFromName (char* cmdName) {
    command_t* currentCommandHandle = lastCommandHandle;
    
    while (currentCommandHandle != NULL) {
        if (strcmp(cmdName, currentCommandHandle->name) == 0) { // strcmp() returns 0 if strings match
            break;
        }
        else currentCommandHandle = currentCommandHandle->prevCommandHandle;
    }
    
    return currentCommandHandle;
}

void processCommand () {
    command_t* commandHandle = getCommandPointerFromName(cmdProcInput.name);
    if (commandHandle != NULL) {
        commandHandle->cmdFunction(cmdProcInput.numParams, cmdProcInput.paramList);
        cmdProcInput.state = CMD_PROC_DONE;
    }
    else printErrorMessages(ERROR_COMMAND_NOT_FOUND);
}

void clearCommandBuffer () {
    for (uint8_t i = 0; i < MAX_COMMAND_SIZE; i++) {
        cmd[i] = BLANK_ARRAY_PLACEHOLDER;
    }
    
    cmdProcInput.numParams = 0;
    cmdProcInput.state = CMD_PROC_IDLE;
}

command_t* getLastCommandHandle () {
    return lastCommandHandle;
}

void printErrorMessages (uint8_t error) {
    if (cmdProcInput.state != CMD_PROC_ERROR) {
        cmdProcInput.state = CMD_PROC_ERROR;
        if (PRINT_ERROR_MESSAGES) {
            switch(error) {
                case ERROR_COMMAND_TOO_LONG:
                    printf("Max command size exceeded. Please modify \"cmdProcessor_config.h\" to allow for longer commands. Clearing command buffer.");
                    break;
                case ERROR_TOO_MANY_PARAMS:
                    printf("Maximum number of parameters (%d) exceeded. Please modify \"cmdProcessor_config.h\" to allow for more parameters.", MAX_NUM_PARAMS);
                    break;
                case ERROR_COMMAND_NOT_FOUND:
                    printf("Command not recognized. Type \"list\" for a list of commands and \"help\" to get usage help about about the command processor.");
                    break;
            }
            printf("\n");
        }
    }
}