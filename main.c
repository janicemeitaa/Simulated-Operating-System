//
//  main.c
//  COMP310FinalAssignment
//
//  Created by Janice Meita on 7/4/2020.
//  Copyright © 2020 Janice Meita. All rights reserved.
//

#include "main.h"
#include "kernel.h"

int main(int argc, char const *argv[]) {
    int error=0;
    boot(); // First : actions performed by boot
    error = kernel(); // Second: actions performed by kernel
    return error;
}
