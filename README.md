# Embedded Command Processor

This is a command processor that can be attached to any serial communication peripheral to run commands on the microcontroller during normal program execution. It comes with 4 built in commands and custom commands can be added by following the user's guide below. <br>
<br>

## Setup and Usage

- `C-Modules-For-Embedded-Development/cmdProcessor.h` must be included in `main.c`.
- `CMD_PROCESSOR_AddCharToBuffer(char charToAdd)` must be called in an interrupt routine to quickly move received characters to a buffer.
- `CMD_PROCESSOR_Init()` must be called before the main loop, inside `main()`.
- `CMD_PROCESSOR_Tasks()` must be called within the main loop.
- `cmdProccessor_config.h' can be edited for customizing the command processor, including changing the maximum command and buffer lengths, as well as the maximum number of parameters.
- Once the program is running, commands can be sent to the microcontroller as line inputs from a serial terminal. The command processor will not accept individual character inputs.
<br>

### Example

The example below uses UART RX complete interrupt to add characters to the command buffer. `byteReceivedInterruptCallback` is a custom function used to handle the UART RX Complete Interrupt. `UART_RxCompleteCallbackRegister` is a function created byt the Microchip Code Configurator for the AVR128DB48. Many parts have such functions for registering callbacks.
<br>

	#include "C-Modules-For-Embedded-Development

	void byteReceivedInterruptCallback (){
		char charToAdd = UART_Read();
		CMD_PROCESSOR_AddCHarToBuffer(charToAdd);
	}

	int main() {
		CMD_PROCESSOR_Init();
		UART_RxCompleteCompleteCallbackRegister(byteReceivedInterruptCallback);

		while (1) {
			CMD_PROCESSSOR_Tasks();
		}
	}

## Built in functions

The built in functions of the command processor are located in `cmdProcessor_builtincommands.h` and are as follows: <br>
- `help`: Prints command processor help
- `list`: Lists all available commands, including user registered ones
- `info <commandName>`: Provides the description and parameter info for `<commandName>`
- `echo <P0> ... <PN>`: Parses the command and prints parsed info

## Config

There are several options that can be changed in the config to customize the command processor. <br>
- `NO_PARAMS_INFO_STRING`: A string to be used when defining commands that do not take any parameters.
- `CMD_PROC_PROMPT_STRING`: The string/character to be used at the beginning of each line where input is expected.
- `CIRCULAR_BUFFER_SIZE`: The size of the circular buffer. Should be more than the maximum command size.
- `MAX_COMMAND_SIZE`: Maximum length for a command, including parameters.
- `MAX_NUM_PARAMS`: Maximum number of parameters allowed for each command.

## Adding Custom Commands

**commands.h** <br>
Every custom command must have a callback function that returns `void` and takes `(uint8_t argc, char* argv[])` as arguments. <br>

	void myCommandFunction (uint8_t argc, char* argv[]);
<br>

**commands.c** <br>
Every custom command must have a struct of type `command_t` associated with it. This struct should contain the following in order: <br>
1. String of command name. Should be one word.
2. Pointer to command callback function.
3. String of the command description.
4. String explaining what parameters the command takes. `NO_PARAM_INFO_STRING` can be used if the command does not take any parameters.
5. This element **MUST** be `NULL`. Otherwise the command processor will not function correctly.

Example:

	static command_t myCmd = {"myCmdName", myCmdFunction, "My command description", "My command parameter information", NULL};

Once the command struct has been created, the command must be registered. This can be done by adding `CMD_PROCESSOR_RegisterCommand(&myCmd);` in the `registerCustomCommands()` function as follows: <br>

	void registerCustomCommands () {
		CMD_PROCESSOR_RegisterCommand(&myCmd);
	}
