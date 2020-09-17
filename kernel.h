//
//  kernel.h
//  COMP310FinalAssignment
//
//  Created by Janice Meita on 7/4/2020.
//  Copyright © 2020 Janice Meita. All rights reserved.
//

#ifndef kernel_h
#define kernel_h

#include <stdio.h>
#include "pcb.h"

/*
 Adds a pcb to the tail of the linked list
 */
void addToReady(struct PCB*);

/*
 Returns the size of the queue
 */
int size(void);

/*
 Pops the pcb at the head of the linked list.
 pop will cause an error if linkedlist is empty.
 Always check size of queue using size()
 */
struct PCB* pop(void);

/*
 Passes a filename
 Opens the file, copies the content in the RAM.
 Creates a PCB for that program.
 Adds the PCB on the ready queue.
 Return an errorCode:
 ERRORCODE 0 : NO ERROR
 ERRORCODE -3 : SCRIPT NOT FOUND
 ERRORCODE -5 : NOT ENOUGH RAM (EXEC)
 */
int myinit(char*);

int scheduler(void);

/*
 Flushes every pcb off the ready queue in the case of a load error
 */
void emptyReadyQueue(void);

void boot(void);

int kernel(void);

#endif /* kernel_h */
