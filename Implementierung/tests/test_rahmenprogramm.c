
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include "../rahmenprogramm.h"

bool test_run_program(int argc, char *argv[], bool expected){
    bool result = run_program(argc, argv);
    if (result == expected){
        return true;
    }
    return false;
}

int main(int argc, char *argv[]){
    for(int i = 0; i < argc; i++){
        printf("%s\n", argv[i]);
    }
}

