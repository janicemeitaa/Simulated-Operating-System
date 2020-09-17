//
//  memorymanager.c
//  COMP310FinalAssignment
//
//  Created by Janice Meita on 7/4/2020.
//  Copyright © 2020 Janice Meita. All rights reserved.
//

#include "memorymanager.h"
#include "pcb.h"
#include "ram.h"
#include "kernel.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int fileNum = 1;
char currentFilePath[100] = "";
char filePrefix[20] = "backup";
char extension[20] = ".txt";
char directory[20] = "BackingStore/";

FILE* copyToBackingStore(FILE* p) {
    //printf("ENTERING COPYTOBACKINGSTORE\n");
    char buffer[1000];
    FILE* outFile;
    char fileName[50] = "";
    sprintf(fileName, "%s%d%s", filePrefix, fileNum, extension);
    //printf("%s\n", fileName);
    sprintf(currentFilePath, "%s%s", directory, fileName);
    //printf("%s\n", currentFilePath);
    char command[100];
    strcpy(command, "touch ");
    strcat(command, currentFilePath);
    system(command);
    outFile = fopen(currentFilePath, "w");
    /*
    if (outFile == NULL) {
        printf("OPEN FILE FOR WRITE FAILED\n");
    } else {
        printf("OPEN FILE SUCCESSFUL\n");
    }
     */
    while (fgets(buffer, sizeof(buffer), p) != NULL) {
        fprintf(outFile, "%s", buffer);
    }
    fclose(outFile);
    FILE * fp = fopen(currentFilePath,"r");
    return fp;
}

int countTotalPages(FILE *f) {
    int lines = 0;
    for (char c = getc(f); c != EOF; c = getc(f)) {
        if (c == '\n') {
            lines = lines + 1;
        }
    }
    if (lines<4) {
        return 1;
    } else {
        int n = (lines+(4-1))/4;
        return n;
    }
}

void loadPage(int pageNumber, FILE *f, int frameNumber) {
    int lineNum = 4*(pageNumber-1);
    int i = 0, j = frameNumber;
    char buffer[1000];
    while ((i<lineNum+4) && (fgets(buffer,999,f) != NULL)) {
        //printf("COMMAND READ AS %s\n", buffer);
        if (i>=lineNum) {
            //printf("IP IS SUPPOSED TO BE %d\n", j);
            ram[j] = strdup(buffer);
            //printf("COMMAND IS SUPPOSED TO BE %s\n", ram[j]);
            j++;
        }
        i++;
    }
}

int findFrame() {
    for (int i=0; i<40; i+=4) {
        if (ram[i]==NULL) {
            return i;
        }
    }
    return -1;
}

int inPageTable(int num, PCB * p) {
    for (int i=0; i<10; i++) {
        if (p->pageTable[i] == num) {
            return 1;
        }
    }
    return 0;
}

int findVictim(PCB *p) {
    int num = (rand() % 10);
    while (inPageTable(num, p) == 1) {
        num++;
        num = num % 10;
    }
    return num;
}

int updatePageTable(PCB *p, int pageNumber, int frameNumber, int victimFrame) {
    if (frameNumber != -1) {
        p->pageTable[pageNumber] = frameNumber;
        return 1;
    }
    if (victimFrame != -1) {
        p->pageTable[pageNumber] = victimFrame;
        return 1;
    }
    return 0;
}

int findNewPage(PCB* pcb, int pageNumber, FILE* f) {
    int frameNumber = -1;
    int victimFrame = -1;
    frameNumber = findFrame();
    if (frameNumber == -1) {
        victimFrame = findVictim(pcb);
        loadPage(pageNumber, f, victimFrame);
        pcb->PC = victimFrame;
    } else {
        loadPage(pageNumber, f, frameNumber);
        pcb->PC = frameNumber;
    }
    return updatePageTable(pcb, pageNumber, frameNumber, victimFrame);
}

int launcher(FILE*p) {
    FILE* newP = copyToBackingStore(p);
    FILE * fp = fopen(currentFilePath,"r");
    fclose(p);
    int pages_max = countTotalPages(fp);
    //printf("PAGES MAX COUNT CHANGED TO %d\n", pages_max);
    PCB* pcb = makePCBForProgram(fileNum, pages_max);
    fileNum++;
    addToReady(pcb);
    for (int i=0; i<2; i++) {
        int check = findNewPage(pcb, i, newP);
        //printf("IS PCB->PC UPDATED?%d\n", pcb->PC);
        if (check == 0) {
            return 0;
        }
    }
    return 1;
}

