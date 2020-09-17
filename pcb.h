//
//  pcb.h
//  COMP310FinalAssignment
//
//  Created by Janice Meita on 7/4/2020.
//  Copyright © 2020 Janice Meita. All rights reserved.
//

#ifndef pcb_h
#define pcb_h

#include <stdio.h>
/*
 PCB has 3 fields
 PC : Points to the the current line reached for that program
 start: Points to first line of that program
 end: Points to last line of that program
 */
typedef struct PCB
{
    int PC;
    int start;
    int end;
    int PID;
    int pageTable[10];
    int PC_page;
    int PC_offset;
    int pages_max;
}PCB;

/*
 Passes 2 parameters (start , end)
 This method creates a PCB with fields set as this:
 PC = start
 start = start
 end = end
 */
PCB* makePCB(int start, int end);

PCB* makePCBForProgram(int PID, int pages_max);

#endif /* pcb_h */
