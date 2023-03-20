#include "commands.h"
#include "../cmdProcessor.h"

/*
 * Custom Command Example - Place in header file (commands.h)
 * 
 * void myCmdFunction (uint8_t argc, char* argv[]);
 */

/*
 * Custom Command Example - Place in this file (commands.c)
 * 
 * static command_t myCmd = {"myCmdName", myCmdFunction, "My command description", "My command parameter information", NULL};
 *                NO_PARAMS_INFO_STRING can be used to indicate that a command takes no parameters^
 * 
 */


void registerCustomCommands () {
    /*
     * Custom Command Example - Place in here
     * 
     * CMD_PROCESSOR_RegisterCommand(&myCmd);
     */
}
