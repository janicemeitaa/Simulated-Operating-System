//
//  interpreter.h
//  COMP310FinalAssignment
//
//  Created by Janice Meita on 7/4/2020.
//  Copyright © 2020 Janice Meita. All rights reserved.
//

#ifndef interpreter_h
#define interpreter_h

#define TRUE 1
#define FALSE 0

/*
 This functions takes a parsed version of the user input.
 It will interpret the valid commands or return a bad error code if the command failed for some reason
 Returns:
 ERRORCODE  0 : No error and user wishes to continue
 ERRORCODE  1 : Users wishes to quit the shell / terminate script
 
 ERRORCODE -1 : RAN OUT OF SHELL MEMORY
 ERRORCODE -2 : INCORRECT NUMBER OF ARGUMENTS
 ERRORCODE -3 : FILE DOES NOT EXIST
 ERRORCODE -4 : UNKNOWN COMMAND. TYPE "help" FOR A MANUAL OF EVERY AVAILABLE COMMANDS
 */
int interpreter(char* words[]);

#endif /* interpreter_h */
