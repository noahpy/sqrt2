
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <getopt.h>
#include <errno.h>


const char* usage_msg = 
    "Usage: %s [options]   Compute the square root of 2\n";

const char* help_msg =
    "Optional arguments:\n"
    "  -V X   The version of the progam (default: X = 0)\n"
    "  -B     Time the runtime\n"
    "  -B X   Time the runtime with X > 0 repetitions (default: X = 1)\n"
    "  -d X   Expected number of decimal places with X >= 0 (default: X = 2)\n"
    "  -h X   Expected number hexadecimal places with X >= 0(default: X = 2)\n"
    "  -h     Show help message (this text) and exit\n"
    "  --help Show help message (this text) and exit\n";


void print_usage(const char* progname) {
    fprintf(stderr, usage_msg, progname);
}

void print_help(const char* progname) {
    print_usage(progname);
    fprintf(stderr, "\n%s", help_msg);
}

int HELP_RETURN_CODE = 9999;

bool convert_string_to_long(const char* str, long* value){
    /*Convert string to long.
     *Returns:
        - value (long): converted string
        - -1 if conversion failed*/
    
    char* endptr;
    errno = 0;
    long result = strtol(str, &endptr, 10);
    if(endptr == str || *endptr){
        fprintf(stderr, "Can not convert %s to double.\n", str);
        return false;
    }
    if(errno == ERANGE){
        fprintf(stderr, "Value %s out of range for long.\n", str);
        return false;
    }
    *value = result;
    return true;
}

bool run_program(int argc, char *argv[]){
    /*Get user options adn trugger matrix exponentiation program.
     *Returns:
        - is_valid (bool): true if user options were vaild, else false*/
    
    const char* progname = argv[0];

    bool timing = false;
    bool decimal_places = false;

    long version = 0;
    long repetitions = 1;
    long decimal_precision = 2;
    long hex_precision = 2;

    int flag = 0;
    struct option long_options[] = {
        {"help", 0, &flag, HELP_RETURN_CODE},
        {0, 0, 0, 0}
    };
    
    int result;
    while((result = getopt_long(argc, argv, "V:B::d:h::", long_options, NULL) )!= -1){
        switch(result){
            case '?':
                print_help(progname);
                return false;
            case 'V':
                if(optarg == NULL){
                    print_help(progname);
                    return false;
                }
                if(!convert_string_to_long(optarg, &version)){
                    print_help(progname);
                    return false;
                }
                break;
            case 'B': 
                timing = true;
                if(optarg != NULL){
                    if(!convert_string_to_long(optarg, &repetitions)){
                        print_help(progname);
                        return false;
                    }
                    if(repetitions <= 0){
                        fprintf(stderr, "Repetitions must be greater than zero.\n");
                        print_help(progname);
                        return false;
                    }
                }
                break;
            case 'd':
                decimal_places = true;
                if(optarg == NULL){
                    print_help(progname);
                    return false;
                }
                if(!convert_string_to_long(optarg, &decimal_precision)){
                    print_help(progname);
                    return false;
                }
                if(decimal_precision < 0){
                    fprintf(stderr, "Decimal precision must be greater than or equal to zero.\n");
                    print_help(progname);
                    return false;
                }
                break;
            case 'h':
                if(optarg != NULL){
                    if(!convert_string_to_long(optarg, &hex_precision)){
                        print_help(progname);
                        return false;
                    }
                    if(hex_precision < 0){
                        fprintf(stderr, "Hex precision must be greater than or equal to zero.\n");
                        print_help(progname);
                        return false;
                    }
                }
                else{
                    print_help(progname);
                    return true;
                }
                break;
            case 0:
                if(flag == HELP_RETURN_CODE){
                    print_help(progname);
                    return true;
                }
                return false;
            default:
                print_help(progname);
                return false;
            }
    }
    // check if there are any non-option arguments
    if(optind < argc  || (optind == 1 && argc > 1)){
        fprintf(stderr, "Found positional arguments\n");
        print_help(progname);
        return false;
    }
    // TODO: Execute program
    return true;
}



