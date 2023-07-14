
#include <stdbool.h>
#include <stdio.h>
#include <stddef.h>
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include "../rahmenprogramm.h"

/**
 * Test Rahmenprogramm with different options
 */


int test_cases = 0;
int test_passed = 0;

void test_run_program(int argc, char *argv[], bool expected){
    test_cases++;

    // copy argv
    char *argv_copy[argc];
    for (int i = 0; i < argc; i++){
        argv_copy[i] = malloc(strlen(argv[i]) + 1);
        strcpy(argv_copy[i], argv[i]);
    }

    // redirect stderr 
    int original_stdout = dup(STDERR_FILENO);
    int dev_null = open("/dev/null", O_WRONLY);
    if(dev_null == -1){
        fprintf(stderr, "Can not open /dev/null\n");
        exit(EXIT_FAILURE);
    }
    if(dup2(dev_null, STDERR_FILENO) == -1){
        fprintf(stderr, "Can not redirect stdout to /dev/null\n");
        exit(EXIT_FAILURE);
    }

    // reset optind 
    optind = 1;
    bool result = run_program(argc, argv);

    // reset stderr
    if(dup2(original_stdout, STDERR_FILENO) == -1){
        fprintf(stderr, "Can not reset stdout\n");
        exit(EXIT_FAILURE);
    }

    if (result == expected){
        test_passed++;
    } else {
        printf("Test failed: run_program(%d, [", argc);
        for (int i = 0; i < argc; i++){
            printf("\"%s\"", argv_copy[i]);
            if (i < argc - 1){
                printf(", ");
            }
        }
        printf("]) == %s, but got %s\n",(expected ? "true":"false"), (result ? "true":"false"));
    }
}

int main(){

    // no options
    test_run_program(1, (char *[]){"./test"}, true);

    // version options
    test_run_program(2, (char *[]){"./test", "-V0"}, true);
    test_run_program(2, (char *[]){"./test", "-V0.0"}, false);
    test_run_program(2, (char *[]){"./test", "-V"}, false);
    test_run_program(2, (char *[]){"./test", "-V3h"}, false);
    test_run_program(2, (char *[]){"./test", "-Vhallo"}, false);
    test_run_program(2, (char *[]){"./test", "-Vhallo58"}, false);

    // -B option: number is optional and number needs to be greater than 0
    test_run_program(2, (char *[]){"./test", "-B190"}, true);
    test_run_program(2, (char *[]){"./test", "-B0"}, false);
    test_run_program(2, (char *[]){"./test", "-B-3"}, false);
    test_run_program(2, (char *[]){"./test", "-B0.0"}, false);
    test_run_program(2, (char *[]){"./test", "-B"}, true);
    test_run_program(2, (char *[]){"./test", "-B3h"}, false);
    test_run_program(2, (char *[]){"./test", "-Bhallo"}, false);
    test_run_program(2, (char *[]){"./test", "-Bhallo58"}, false);

    // -d option: number needs to be greater equal 0
    test_run_program(2, (char *[]){"./test", "-d0"}, true);
    test_run_program(2, (char *[]){"./test", "-d12"}, true);
    test_run_program(2, (char *[]){"./test", "-d12446"}, true);
    test_run_program(2, (char *[]){"./test", "-d0.0"}, false);
    test_run_program(2, (char *[]){"./test", "-d-2"}, false);
    test_run_program(2, (char *[]){"./test", "-d"}, false);
    test_run_program(2, (char *[]){"./test", "-d3h"}, false);
    test_run_program(2, (char *[]){"./test", "-dhallo"}, false);
    test_run_program(2, (char *[]){"./test", "-dhallo58"}, false);

    // -h option: does also accept numbers which are greater equal 0
    test_run_program(1, (char *[]){"./test", "-h"}, true);
    test_run_program(2, (char *[]){"./test", "-h0"}, true);
    test_run_program(2, (char *[]){"./test", "-h12446"}, true);
    test_run_program(2, (char *[]){"./test", "-h0.0"}, false);
    test_run_program(2, (char *[]){"./test", "-h-34"}, false);
    test_run_program(2, (char *[]){"./test", "-h3h"}, false);
    test_run_program(2, (char *[]){"./test", "-hhallo"}, false);
    test_run_program(2, (char *[]){"./test", "-hhallo58"}, false);

    // --help option
    test_run_program(2, (char *[]){"./test", "--help"}, true);
    test_run_program(3, (char *[]){"./test", "0", "--help"}, true);
    test_run_program(3, (char *[]){"./test", "--help", "0"}, true);

    // combine different options
    test_run_program(3, (char *[]){"./test", "-h1", "-V0"}, true);
    test_run_program(3, (char *[]){"./test", "-h1", "-V0.0"}, false);
    test_run_program(4, (char *[]){"./test", "-h1", "-V0", "-d0"}, true);
    test_run_program(4, (char *[]){"./test", "-h1", "-V0", "-d0.0"}, false);
    test_run_program(5, (char *[]){"./test", "-h1", "-V0", "-d0", "-B1"}, true);
    test_run_program(5, (char *[]){"./test", "-h1", "-V0", "-d0", "-B"}, true);
    test_run_program(5, (char *[]){"./test", "-h1", "-V0", "-d0", "-B0.0"}, false);
    test_run_program(5, (char *[]){"./test", "-h1er", "-V0", "-d0", "-B0.0"}, false);
    test_run_program(4, (char *[]){"./test", "-V0", "-d0", "-B"}, true);

    // unknown options
    test_run_program(2, (char *[]){"./test", "-x"}, false);
    test_run_program(2, (char *[]){"./test", "-x0"}, false);
    test_run_program(2, (char *[]){"./test", "--hello"}, false);

    // combine with unknown options
    test_run_program(3, (char *[]){"./test", "-h1", "-x0"}, false);
    test_run_program(4, (char *[]){"./test", "-h1", "-V0", "-x"}, false);
    test_run_program(5, (char *[]){"./test", "-h1", "-V0", "-d0", "-x"}, false);

    // positional arguments
    test_run_program(2, (char *[]){"./test", "0"}, false);
    test_run_program(2, (char *[]){"./test", "help"}, false);

    // combine with positional arguments
    test_run_program(3, (char *[]){"./test", "0", "-B"}, false);
    test_run_program(3, (char *[]){"./test", "-V0", "0"}, false);
    
    // print overall result
    float success_rate = ((float)test_passed)/((float)test_cases) * 100;
    printf("\nPASSED: %d, FAILED: %d, SUCESS RATE: %.1f%%\n", test_passed, (test_cases-test_passed), success_rate);

}


