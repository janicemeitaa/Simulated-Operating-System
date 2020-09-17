//
//  cpu.h
//  COMP310FinalAssignment
//
//  Created by Janice Meita on 7/4/2020.
//  Copyright © 2020 Janice Meita. All rights reserved.
//

#ifndef cpu_h
#define cpu_h

#define DEFAULT_QUANTA 2

struct CPU {
    int IP;
    char IR[1000];
    int quanta;
} CPU;

int run(int quanta, PCB* pcb);

#endif /* cpu_h */
