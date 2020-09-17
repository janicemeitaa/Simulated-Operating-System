//
//  memorymanager.h
//  COMP310FinalAssignment
//
//  Created by Janice Meita on 7/4/2020.
//  Copyright © 2020 Janice Meita. All rights reserved.
//

#ifndef memorymanager_h
#define memorymanager_h

#include <stdio.h>
#include "pcb.h"

int fileNum;
char currentFilePath[100];
char filePrefix[20];
char extension[20];
char directory[20];

FILE* copyToBackingStore(FILE* p);
int countTotalPages(FILE *f);
void loadPage(int pageNumber, FILE *f, int frameNumber);
int findFrame(void);
int inPageTable(int num, PCB * p);
int findVictim(PCB *p);
int findNewPage(PCB* pcb, int pageNumber, FILE* f);
int launcher(FILE *p);

#endif /* memorymanager_h */
