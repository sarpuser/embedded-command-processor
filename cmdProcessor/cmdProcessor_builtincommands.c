#include <stdio.h>
#include <stdint.h>

#include "cmdProcessor.h"
#include "cmdProcessor_config.h"
#include "cmdProcessor_private.h"
#include "cmdProcessor_builtincommands.h"

static command_t helpCmd = {"help", help, "Prints command processor help", NO_PARAMS_INFO_STRING, NULL};
static command_t listCmd = {"list", list, "Lists the available commands", NO_PARAMS_INFO_STRING, NULL};
static command_t infoCmd = {"info", info, "Prints detailed command information", "<cmdName>: Name of command", NULL};
static command_t echoCmd = {"echo", echo, "Parses command and prints parsed info", "<P0> ... <PN>: Parameters to parse", NULL};

void registerBuiltinCommands () {
    CMD_PROCESSOR_RegisterCommand(&helpCmd);
    CMD_PROCESSOR_RegisterCommand(&listCmd);
    CMD_PROCESSOR_RegisterCommand(&infoCmd);
    CMD_PROCESSOR_RegisterCommand(&echoCmd);
}

void help (uint8_t argc, char* argv[]) {
    printf("Command processor help:\n");
    printf("\t- This command processor only works with line inputs and will not accept single character inputs\n");
    printf("\t- Type \"list\" to see a list of available commands\n");
    printf("\t- Type \"info <commandName>\" to see detailed help for <commandName>\n");
}

void list (uint8_t argc, char* argv[]) {
    command_t* currentCommandHandle = getLastCommandHandle();
    
    printf("List of commands available:\n");
    
    while (currentCommandHandle != NULL) {
        printf("\t- %s: %s\n", currentCommandHandle->name, currentCommandHandle->description);
        currentCommandHandle = currentCommandHandle->prevCommandHandle;
    }
}

void info (uint8_t argc, char* argv[]){
    command_t* cmd = getCommandPointerFromName(argv[0]);
    
    if (argc == 1){
        if (cmd != NULL) {
            printf("Command info: %s\n", cmd->name);
            printf("\t- Description: %s\n", cmd->description);
            printf("\t- Parameter info: %s\n", cmd->paramInfo);
        }
        else printf("Command \"%s\" not found. Type \"list\" to see a list of available commands.\n", argv[0]);
    }
    else printf("The info command requires the name of a command as its single argument.\n");
}

void echo (uint8_t argc, char* argv[]) {
    if (argc > 0) {
        printf("Command name: %s\n", argv[0]);
        if (argc > 1) {
            printf("Parameters (%d):\n", argc - 1);
            for (uint8_t i = 1; i < argc; i++) {
                printf("\t- Parameter %d: %s\n", i, argv[i]);
            }
        }
    }
    else printf("The echo command requires at least one parameter.");
}