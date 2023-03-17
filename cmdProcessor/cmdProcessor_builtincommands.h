/* 
 * File:   cmdProcessor_builtincommands.h
 * Author: c72700
 *
 * Created on March 14, 2023, 9:18 PM
 */

#ifndef CMDPROCESSOR_BUILTINCOMMANDS_H
#define	CMDPROCESSOR_BUILTINCOMMANDS_H

#include <stdint.h>

void registerBuiltinCommands ();

void help (uint8_t argc, char* argv[]);
void list (uint8_t argc, char* argv[]);
void info (uint8_t argc, char* argv[]);
void echo (uint8_t argc, char* argv[]);

#endif	/* CMDPROCESSOR_BUILTINCOMMANDS_H */

