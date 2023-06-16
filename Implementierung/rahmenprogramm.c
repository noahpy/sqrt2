
#include <stdlib.h>
#include <stdbool.h>


const char* usage_msg = 
    "Usage: %s [options]   Compute the square root of 2\n";

const char* help_msg =
    "Optional arguments:\n"
    "  -V X   The version of the progam (default: X = 0)\n"
    "  -B     Time the runtime\n"
    "  -B X   Time the runtime with X repetitions (default: X = 1)\n"
    "  -d X   Expected number of decimal places (default: X = 2)\n"
    "  -h X   Expected number hexadecimal places (default: X = 2)\n"
    "  -h     Show help message (this text) and exit\n"
    "  --help     Show help message (this text) and exit\n";


bool run_program(int argc, char *argv[]){
    /*Get user options adn trugger matrix exponentiation program.
     *Returns:
        - is_valid (bool): true if user options were vaild, else false*/
    (void) argc;
    (void) argv;
    return true;
}



