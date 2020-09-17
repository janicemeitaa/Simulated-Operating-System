//
//  pcb.c
//  COMP310FinalAssignment
//
//  Created by Janice Meita on 7/4/2020.
//  Copyright © 2020 Janice Meita. All rights reserved.
//

#include "pcb.h"
#include <stdio.h>
#include <stdlib.h>
/*
 PCB has 3 fields
 PC : Points to the the current line reached for that program
 start: Points to first line of that program
 end: Points to last line of that program
 */
typedef struct PCB PCB;


/*
 Passes 2 parameters (start , end)
 This method creates a PCB with fields set as this:
 PC = start
 start = start
 end = end
 */
PCB* makePCB(int start, int end){
    PCB* pcb = (PCB*)malloc(sizeof(PCB));
    pcb->PC = start;
    pcb->start = start;
    pcb->end = end;
    return pcb;
}

PCB* makePCBForProgram(int PID, int pages_max) {
    PCB* pcb = (PCB*)malloc(sizeof(PCB));
    pcb->PID = PID;
    pcb->PC_page = 1;
    pcb->PC_offset = 0;
    pcb->pages_max = pages_max;
    for (int i=0; i<10; i++) {
        pcb->pageTable[i] = -1;
    }
    return pcb;
}
