#include "ane.h"

double* 
parseInput(const char* input, int* call_size) {
    double* call = malloc(sizeof(*call) * 1000);
    
    int call_pointer = 0; // Total Number Of Elements In The Call Array
    const char* sp = input; // String Pointer

    double val; // Place Values Used For Parsing 
    int char_elapsed;
    char arg[100];
    while(*sp != '\0') {
        printf("%c ", *sp);

        if(*sp >= '0' && *sp <= '9') {

            sscanf(sp, "%lg%n", &val, &char_elapsed);
            sp += char_elapsed;
            call[call_pointer++] = val;
        }
        else {
            if(*sp == ' ') // Moving On If We Are At A Space
                break;

            sscanf(sp, "%s%n", arg, &char_elapsed);
            sp += char_elapsed;

            if(strcmp(arg, "dup") == 0)
                call[call_pointer++] = makeBox(DUP, PREBUILT);
            
            sp++;
            }
        }
        printf("\n");
        *call_size = call_pointer;
        return call; 
    }
