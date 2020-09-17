//
//  cpu.c
//  COMP310FinalAssignment
//
//  Created by Janice Meita on 7/4/2020.
//  Copyright © 2020 Janice Meita. All rights reserved.
//

#include<string.h>
#include "ram.h"
#include "interpreter.h"
#include "shell.h"
#include "pcb.h"

#define DEFAULT_QUANTA 2

struct CPU {
    int IP;
    int offset;
    char IR[1000];
    int quanta;
} CPU;

/*
 This method passes a quanta.
 It will then executes quanta number of lines in RAM as from the address
 pointed by the CPU IP.
 Returns an errorCode.
 */
int run(int quanta, PCB* pcb){
    //printf("ENTERING RUN, CHECK QUANTA %d\n", quanta);
    CPU.offset = pcb->PC_offset + 1;
    //printf("CHECK PCB OFFSET %d\n", CPU.offset);
    
    // If a quanta greater than the default quanta of 2 is passed,
    // run will instead execute only default quanta of lines.
    if (quanta > DEFAULT_QUANTA ){
        quanta = DEFAULT_QUANTA;
    }
    
    for (int i = 0; i < quanta; i++)
    {
        //printf("CHECK ITERATION %d\n", i);
        strcpy(CPU.IR,ram[CPU.IP]);
        //printf("CHECK IF RAM IS LOADED%s\n", ram[CPU.IP]);
        int errorCode = parse(CPU.IR);
        // Do error checking and if error, return error
        if (errorCode != 0){
            // Display error message if fatal
            if (errorCode < 0){
                ram[CPU.IP][strlen(ram[CPU.IP])-2]='\0';
                displayCode(errorCode,ram[CPU.IP]);
            }
            return errorCode;
        }
        if (CPU.offset == 4) {
            // Page fault
            //printf("ENTER PAGE FAULT\n");
            pcb->PC_offset = CPU.offset-1;
            return -4;
        }
        CPU.offset ++;
        //printf("CPU.OFFSET CHANGED TO%d\n", CPU.offset);
        CPU.IP ++;
        //printf("CPU.IP CHANGED TO%d\n", CPU.IP);
    }
    pcb->PC_offset = CPU.offset-1;
    return 0;
}
