/* This file contains functions that accept each of the command fields: ~command type~, ~sets~, ~initialization values~.
 * */
#include "set.h"

/*The following function accepts a pointer to a pointer, reads the command type, and initializes the pointer to point to the resulting function.
numOfArg - the number of arguments (sets) needed for the received command.
If the command type is not recognized, an error message will be printed accordingly,
The 'validInput' flag will be turned on, and this will cause the getCommand program to skip to the next command
 */
void readCommandType(void** pointerToFunc, Vars* vars)
{

    vars->commaRequired=0; /* There shouldn't be a comma here */
    vars->command = scanWord(vars);
    if(vars->isEOF || vars->endLine) /* If the line is empty */
        vars->emptyLine=1; /* Turn on the flag */
    else if(vars->command != NULL) {
        if ((strcmp(vars->command, readStr)) == 0) {
            *pointerToFunc = read_set;
            vars->numOfArg = 1;
            vars->command = readStr;
        } else if (strcmp(vars->command, printSetStr) == 0) {
            *pointerToFunc = &print_set;
            vars->numOfArg = 1;
            vars->command = printSetStr;
        } else if (strcmp(vars->command, unionSetStr) == 0) {
            *pointerToFunc = &union_set;
            vars->numOfArg = 3;
            vars->command = unionSetStr;
        } else if (strcmp(vars->command, intersectSetStr) == 0) {
            *pointerToFunc = &intersect_set;
            vars->numOfArg = 3;
            vars->command = intersectSetStr;
        } else if (strcmp(vars->command, symdiffSetStr) == 0) {
            *pointerToFunc = &symdiff_set;
            vars->numOfArg = 3;
            vars->command = symdiffSetStr;
        } else if (strcmp(vars->command, subSetStr) == 0) {
            *pointerToFunc = &sub_set;
            vars->numOfArg = 3;
            vars->command = subSetStr;
        } else if (strcmp(vars->command, stopStr) == 0) {
            vars->stopSign = 1;
            vars->command = stopStr;
        } else {
            vars->validInput = 0;
            puts("Undefined command name");
        }
    }
}
/*reads the operands as sets
The number of sets it reads - the value of numOfArg
First you check the first 3 characters of the operand, if they are equal to "SET", then checks the 4th character, if it is in the range of [A-F]
If the detected operand is not a valid set, an error message is printed accordingly, the 'validInput' flag is set, and this will cause the getCommand function to skip to the next command line
 * */
void readOperands(Vars* vars)
{
    char *SETStr="SET";
    vars->isEOF=0;
    int num = vars->numOfArg;
    set **p; /* will point to the addresses of the operands, and with its help the operands will be initialized to the appropriate value */
    int i=0;
    char *op; /* will receive the string that represents the operand */
    p = &vars->operandA;
    while(i<num){ /* num - The required number of arguments */
        if (i==1) {
            if(!commaDetected(vars)){
                if(vars->isEOF || vars->endLine) puts("Missing parameter");
                vars->validInput=0;
                break;
            } /* Required comma before the second operand */
            p = &vars->operandB;
        }
        else if (i==2){
            if(!commaDetected(vars)) {
                if(vars->isEOF || vars->endLine) puts("Missing parameter");
                vars->validInput = 0;
                break;
            }/* Required comma before the third operand */
            p=&vars->operandC;
        }
/*    	if(i+1 == num && strcmp(vars->command,readStr) != 0) vars->endLine=1; // sign the flag if is the last operand */
        op = scanWord(vars);
        if(op==NULL) break; /* If no suitable text is detected */
        if(strncmp(op,SETStr,3)==0 && 'A' <= *(op+3) && *(op+3)<='F' && strlen(op)==4) {
            if (*(op+3) == 'A') *p = vars->SETA;
            else if (*(op+3) == 'B') *p = vars->SETB;
            else if (*(op+3) == 'C') *p = vars->SETC;
            else if (*(op+3) == 'D') *p = vars->SETD;
            else if (*(op+3) == 'E') *p = vars->SETE;
            else if (*(op+3) == 'F') *p = vars->SETF;
        }
        else{ /* If the operand is not exist */
            vars->validInput=0;
            puts("Undefined set name");
            break;
        }
        i++;
    }
}
