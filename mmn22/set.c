/*
 *This file contains all the functions called by the user, and auxiliary functions.
 *For the sake of order, all functions called by the user receive 3 sets, and thus the 'p_set' pointer corresponds to any function of the above type. When a function receives a parameter that it does not use, the parameter will read 'nothing'.
 * Since each set consists of 4 ints, each operation on a set is done in 4 parts.
 * Each of the outer functions will be sent the address of vars, so that the functions can access the variables/flags.
 */
#include "set.h"

/*#1 - Receives a set, and numbers in the range [0-127] and inserts the received numbers into the set, '-1' is sign to the end of the numbers*/
void read_set(set s, set nothing1, set nothing2, Vars* vars) {
    set setForMask={0}; /*For mask that just the n-th bit is 1*/
    set temp = {0};
    int n = 0;
    while (n!=-1) {
        /*Checks whether a comma is detected before the next number, as required*/
        if (!commaDetected(vars) || vars->isEOF || vars->endLine) { /*A case where the input ended with a character other than '-'*/
            puts("List of set members is not terminated correctly");
            vars->validInput = 0;
            break;
        }
        else {
            n = scanNumber(vars); /*Read the next number*/
            if (n == (int) NULL) { /*Case of error in the input*/
                if (vars->isEOF || vars->endLine)
                    puts("List of set members is not terminated correctly");
                vars->validInput = 0;
                break;
            }
            if ((n < 0 || n > 127) && n != -1) {
                puts("Invalid set member - value out of range");
                vars->validInput = 0;
                break;
            }
            if (n == -1) {
                break;
            }
            sub_set(setForMask, setForMask, setForMask, vars); /*Reset the set*/
            nMask(setForMask, n); /*creating mask that just the n-th bit is 1*/
            union_set(temp, setForMask, temp, vars); /*insert the number 'n' to 'temp' set*/
        }
        /*Initialize s just if all the members are valid*/
    }
    if(vars->validInput)
        union_set(temp, temp, s, vars); /*Copy temp to s*/
}
/*#2 - Prints the members of the set 's'*/
void print_set(set s, set nothing1, set nothing2, Vars* vars) {
    int counter = 0; /*To check how many members were printed in a line*/
    set temp = {0}, mask = {0}; /*temp - to contain all each member in all each iteration, if*/
    int i = 0;
    if (empty_set(s))
        puts("The set is empty");
    else{
        /*Goes through the 128 bits, checks with a mask if the i-th bit is on, if so, prints the number is exists*/
        for (; i < 128; i++) {
            nMask(mask, i); /*creating mask that just the i-th bit is 1*/
            intersect_set(s, mask, temp, vars); /*If the i-th bit exist in 's', the i-th bit int temp will turn on*/
            if (!empty_set(temp)) {
                if (counter > 0 && counter % MAX_MEMBERS_IN_LINE == 0) {
                    puts("");
                } /*If the maximum members (as defined) already printed, down line*/
                counter == 0 ? printf("{%i", i) : printf(", %i", i); /*Checks if this is the first element*/
                counter++;
            }
        }
        printf("}\n");
    }
}
/*
Each of the following 4 commands are performed using the bitwise operation.
The use of the bitwise operation on each group will be carried out on each of the 4 integers (of which the group structure is composed) separately.
Bit j in set S is turn on, meaning the member j available in set S
*/

/*#3 - The command performs a union of set A with set B, and stores the result in group C*/
void union_set(set a, set b, set c, Vars* vars){
    int i=0;
    for(; i<4; i++)
        *(c+i) = *(a+i) | *(b+i); /*Turns on bits in C if they are in A or in B*/
}
/*#4 - The command performs an intersection of set A with set B and stores the result in set C*/
void intersect_set(set a, set b, set c, Vars* vars){
    int i=0;
    for(; i<4; i++)
        *(c+i) = *(a+i) & *(b+i); /*Turns on in C the bits that are in both A and B*/
}
/*#5 - The command subtracts set B from set A, and stores the result in set C.*/
void sub_set(set a, set b, set c, Vars* vars){
    int i=0;
    for(; i<4; i++)
        *(c+i) = *(a+i) & (*(a+i)^*(b+i)); /*Turns on bits in C if they are in A and not in B*/
}
/*#6 - The command calculates the symmetrical difference of set A and set B, and stores the result in set C.*/
void symdiff_set(set a, set b, set c, Vars* vars){
    int i=0;
    for(; i<4; i++)
        *(c+i) = *(a+i) ^ *(b+i); /*Turns on bits in C if they are in A and not in B, or if they are in B and not in A*/
}

/* 		 ~~~~~~~~~~~~~~~~ Auxiliary functions ~~~~~~~~~~~~~~~~	*/

/*Returns 0 if the set is empty, and 1 otherwise*/
int empty_set(set s){
    int result=1;
    int i=0;
    for(; i<4; i++)
        if(*(s+i) != 0) { /*If an on bit is detected, the set is not empty*/
            result = 0;
            break;
        }
    return result;
}
/*Creates mask of 128 bits that just the n-th bit is 1*/
void nMask(set s, int n){
    if (n<0 || 127<n) /*The possible range is [0-127]*/
        puts("nMask function gets only number in a range [0-127]!");
        /*checkin if the number 'n' is in range [0-31], [32-63], [64-95] or [96-127] (in accordance to the partition of 4 blocks of integers)*/
    else if(n<=31)
        s[0]= pow(2, n);
    else if(n<=63)
        s[1] = pow(2,n-32);
    else if(n<=95)
        s[2] = pow(2, n-64);
    else /*if 96 <= n <= 127*/
        s[3] = pow(2, n-96);
}
/*Skips to the next line, i.e. until the '\n' character or enter is detected*/
void nextLine(Vars* vars){
    char ch;
    if(vars->lastChar != '\n') { /*If is not already the end of line*/
        ch = getchar();
        while (ch != '\n' && ch != ENTER) { /*Run on the line until the end of line character is detected*/
            ch = getchar();
        }
    }
    vars->endLine = 0; /* Reset the flag */
}
/*Checks if a redundant character has been detected between the command fields
 Returns 1 if yes, and 0 otherwise
 Type 1 - the first check, after the command field
 Type 2 - the first check, after the operands fields
 */
int redundantChar(int type, Vars* vars){
    char* tailChars;
    int result;
    if(type == 2){ /*If this is the second check*/
        if(strcmp(vars->command, readStr)==0 || vars->endLine) /*No relevant in this case, because of the numbers for reading is not redundant*/
            result=0;
        else {
            tailChars = scanWord(vars);
            if (tailChars != NULL) {
                puts("Extraneous text after end of command");
                result = 1;
            }
        }
    }
    else { /*if type == 1*/
        if (vars->lastChar == ' ' || vars->lastChar == '\t') /*Valid characters that can appear between command fields*/
            result = 0;
        else {
            vars->validInput = 0; /*Indicates that the input is incorrect*/
            if (vars->lastChar == ',')
                puts("Illegal comma");
            result = 1;
        }
    }
    return result;
}
/*Receives a variable of type vars and initializes all variables to the diplative values*/
void varsToDefaulte(Vars* vars)
{
    vars->commaRequired=0;
    vars->validInput=1;
    vars->lastChar=' ';
    vars->isEOF=0;
    vars->endLine=0;
    vars->numOfArg=0;
    vars->stopSign=0;
    vars->emptyLine=0;
    resetTheSet(vars->SETA);
    resetTheSet(vars->SETB);
    resetTheSet(vars->SETC);
    resetTheSet(vars->SETD);
    resetTheSet(vars->SETE);
    resetTheSet(vars->SETF);
}
/*Receives a group and resets it*/
void resetTheSet(set s){
    int i=0;
    while(i<4)
        s[i++] = 0;
}
