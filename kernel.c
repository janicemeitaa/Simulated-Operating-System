//
//  kernel.c
//  COMP310FinalAssignment
//
//  Created by Janice Meita on 7/4/2020.
//  Copyright © 2020 Janice Meita. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shell.h"
#include "pcb.h"
#include "ram.h"
#include "cpu.h"
#include "interpreter.h"
#include "memorymanager.h"

/*
 This is a node in the Ready Queue implemented as
 a linked list.
 A node holds a PCB pointer and a pointer to the next node.
 PCB: PCB
 next: next node
 */
typedef struct ReadyQueueNode {
    PCB*  PCB;
    struct ReadyQueueNode* next;
} ReadyQueueNode;

ReadyQueueNode* head = NULL;
ReadyQueueNode* tail = NULL;
int sizeOfQueue = 0;

/*int main(int argc, char const *argv[])
{
    shellUI();
}*/
/*
 Adds a pcb to the tail of the linked list
 */
void addToReady(struct PCB* pcb) {
    ReadyQueueNode* newNode = (ReadyQueueNode *)malloc(sizeof(ReadyQueueNode));
    newNode->PCB = pcb;
    newNode->next = NULL;
    if (head == NULL){
        head = newNode;
        tail = newNode;
    } else {
        tail->next = newNode;
        tail = newNode;
    }
    sizeOfQueue++;
}

/*
 Returns the size of the queue
 */
int size(){
    return sizeOfQueue;
}

/*
 Pops the pcb at the head of the linked list.
 pop will cause an error if linkedlist is empty.
 Always check size of queue using size()
 */
struct PCB* pop(){
    PCB* topNode = head->PCB;
    ReadyQueueNode * temp = head;
    if (head == tail){
        head = NULL;
        tail = NULL;
    } else {
        head = head->next;
    }
    free(temp);
    sizeOfQueue--;
    return topNode;
}

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
int myinit(char* filename){
    // Open the filename to get FILE *
    // call addToRam on that File *
    // If error (check via start/end variable), return that error
    // Else create pcb using MakePCB
    // Then add it to the ReadyQueue
    FILE * fp = fopen(filename,"r");
    if (fp == NULL) return -3;
    int start;
    int end;
    addToRAM(fp,&start,&end);
    fclose(fp);
    if (start == -1) return -5;
    PCB* pcb = makePCB(start,end);
    addToReady(pcb);
    return 0;
}

void clearBackingStore() {
    char command[50];
    strcpy(command, "rm -rf BackingStore/*");
    system(command);
}

int scheduler(){
    // set CPU quanta to default, IP to -1, IR = NULL
    CPU.quanta = DEFAULT_QUANTA;
    CPU.IP = -1;
    while (size() != 0){
        //pop head of queue
        PCB* pcb = pop();
        //printf("CHECK PAGES_MAX %d\n", pcb->pages_max);
        //copy PC of PCB to IP of CPU
        //printf("CHECK PCB->PC %d\n", pcb->PC);
        //printf("CHECK PCB->PC_OFFSET %d\n", pcb->PC_offset);
        CPU.IP = pcb->PC + pcb->PC_offset;
        //printf("IP IS SET AS %d\n", CPU.IP);
        //printf("CHECK IF RAM IS LOADED%s\n", ram[CPU.IP]);
        
        int isOver = FALSE;
        int remaining = pcb->pages_max + 1;
        int quanta = DEFAULT_QUANTA;
        
        if (DEFAULT_QUANTA >= remaining) {
            isOver = TRUE;
            quanta = remaining;
        }
        
        //printf("BEFORE ENTERING RUN, CHECK QUANTA %d\n", quanta);
        int errorCode = run(quanta, pcb);
        
        if (errorCode == -4) {
            pcb->PC_page++;
            //printf("CHECK PCB->PC_PAGE IN PAGE FAULT %d\n", pcb->PC_page);
            if (pcb->PC_page > pcb->pages_max) {
                removeFromRam(pcb->start,pcb->end);
                free(pcb);
            } else {
                if (pcb->pageTable[pcb->PC_page] != -1) {
                    int frame = pcb->pageTable[pcb->PC_page];
                    pcb->PC = frame;
                    pcb->PC_offset = 0;
                    addToReady(pcb);
                } else {
                    //printf("REOPENING FILE IN PAGE FAULT\n");
                    char fullPath[50];
                    char name[20];
                    sprintf(name, "%s%d%s", filePrefix, pcb->PID, extension);
                    sprintf(fullPath, "%s%s", directory, name);
                    //printf("CHECK FILE PATH AGAIN %s\n", fullPath);
                    FILE * fp = fopen(fullPath,"r");
                    findNewPage(pcb, pcb->PC_page, fp);
                    int frame = pcb->pageTable[pcb->PC_page];
                    pcb->PC = frame;
                    pcb->PC_offset = 0;
                    addToReady(pcb);
                }
            }
        } else if ( errorCode!=0 || isOver ){
            removeFromRam(pcb->start,pcb->end);
            free(pcb);
        } else {
            /*pcb->PC += DEFAULT_QUANTA;
            printf("VALUE OF PCB->PC AFTER RUN %d\n", pcb->PC);*/
            addToReady(pcb);
        }
    }
    // reset RAM
    resetRAM();
    clearBackingStore();
    return 0;
}

/*
 Flushes every pcb off the ready queue in the case of a load error
 */
void emptyReadyQueue(){
    while (head!=NULL){
        ReadyQueueNode * temp = head;
        head = head->next;
        free(temp->PCB);
        free(temp);
    }
    sizeOfQueue =0;
}

void boot() {
    for (int i = 0; i <= 40; i++)
    {
        ram[i] = NULL;
    }
    char command[50];
    strcpy(command, "rm -rf BackingStore");
    system(command);
    strcpy(command, "mkdir BackingStore");
    system(command);
}

int kernel() {
    int errorCode = shellUI();
    switch (errorCode)
    {
        case -1:
            printf("ERRORCODE -1 : RAN OUT OF SHELL MEMORY\n");
            break;
        case -2:
            printf("ERRORCODE -2 : INCORRECT NUMBER OF ARGUMENTS. TRY 'help'\n");
            break;
        case -3:
            printf("ERRORCODE -3 : SCRIPT NOT FOUND\n");
            break;
        case -4:
            printf("ERRORCODE -4 : IS AN UNKNOWN COMMAND. TRY 'help'\n");
            break;
        case -5:
            printf("ERRORCODE -5 : NOT ENOUGH RAM TO ADD PROGRAM.\n");
            break;
        case -6:
            printf("ERRORCODE -6 : ALREADY LOADED \n");
            break;
    }
    return errorCode;
}
