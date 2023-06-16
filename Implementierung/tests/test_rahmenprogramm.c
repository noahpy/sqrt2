
#include <stdbool.h>
#include <stdio.h>
#include <stddef.h>
#include "../rahmenprogramm.h"


int test_cases = 0;
int test_passed = 0;

void test_run_program(int argc, char *argv[], bool expected){
    test_cases++;
    bool result = run_program(argc, argv);
    if (result == expected){
        test_passed++;
    } else {
        printf("Test failed: run_program(%d, [", argc);
        for (int i = 0; i < argc; i++){
            printf("\"%s\"", argv[i]);
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
    test_run_program(2, (char *[]){"./test", "-V", "0"}, true);
    test_run_program(3, (char *[]){"./test", "-V", "0.0"}, false);
    test_run_program(2, (char *[]){"./test", "-V"}, false);
    test_run_program(3, (char *[]){"./test", "-V", "3h"}, false);
    test_run_program(3, (char *[]){"./test", "-V", "hallo"}, false);
    test_run_program(3, (char *[]){"./test", "-V", "hallo58"}, false);
    // -B option: number is optional
    test_run_program(3, (char *[]){"./test", "-B", "0"}, true);
    test_run_program(3, (char *[]){"./test", "-B", "0.0"}, false);
    test_run_program(2, (char *[]){"./test", "-B"}, true);
    test_run_program(3, (char *[]){"./test", "-B", "3h"}, false);
    test_run_program(3, (char *[]){"./test", "-B", "hallo"}, false);
    test_run_program(3, (char *[]){"./test", "-B", "hallo58"}, false);
    // -d option
    test_run_program(3, (char *[]){"./test", "-d", "0"}, true);
    test_run_program(3, (char *[]){"./test", "-d", "12446"}, true);
    test_run_program(3, (char *[]){"./test", "-d", "0.0"}, false);
    test_run_program(2, (char *[]){"./test", "-d"}, false);
    test_run_program(3, (char *[]){"./test", "-d", "3h"}, false);
    test_run_program(3, (char *[]){"./test", "-d", "hallo"}, false);
    test_run_program(3, (char *[]){"./test", "-d", "hallo58"}, false);
    // -h option: does also accept numbers
    test_run_program(2, (char *[]){"./test", "-h"}, true);
    test_run_program(3, (char *[]){"./test", "-h", "0"}, true);
    test_run_program(3, (char *[]){"./test", "-h", "12446"}, true);
    test_run_program(3, (char *[]){"./test", "-h", "0.0"}, false);
    test_run_program(3, (char *[]){"./test", "-h", "3h"}, false);
    test_run_program(3, (char *[]){"./test", "-h", "hallo"}, false);
    test_run_program(3, (char *[]){"./test", "-h", "hallo58"}, false);
    // --help option
    test_run_program(2, (char *[]){"./test", "--help"}, true);
    test_run_program(3, (char *[]){"./test", "--help", "0"}, false);
    test_run_program(3, (char *[]){"./test", "--help", "hilfe"}, false);
    test_run_program(3, (char *[]){"./test", "--help", "0.0"}, false);

    // combine different options
    test_run_program(5, (char *[]){"./test", "-h", "1", "-V", "0"}, true);
    test_run_program(5, (char *[]){"./test", "-h", "1", "-V", "0.0"}, false);
    test_run_program(7, (char *[]){"./test", "-h", "1", "-V", "0", "-d", "0"}, true);
    test_run_program(7, (char *[]){"./test", "-h", "1", "-V", "0", "-d", "0.0"}, false);
    test_run_program(9, (char *[]){"./test", "-h", "1", "-V", "0", "-d", "0", "-B", "1"}, true);
    test_run_program(8, (char *[]){"./test", "-h", "1", "-V", "0", "-d", "0", "-B"}, true);
    test_run_program(9, (char *[]){"./test", "-h", "1", "-V", "0", "-d", "0", "-B", "0.0"}, false);
    test_run_program(9, (char *[]){"./test", "-h", "1er", "-V", "0", "-d", "0", "-B", "0.0"}, false);
    test_run_program(5, (char *[]){"./test", "-V", "0", "-d", "0", "-B"}, true);

    // print overall result
    float success_rate = ((float)test_passed)/((float)test_cases) * 100;
    printf("PASSED: %d, FAILED: %d, SUCESS RATE: %.1f%%\n", test_passed, (test_cases-test_passed), success_rate);

}


