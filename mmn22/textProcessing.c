/*This file contains functions that receive text input and process it.
 * */

#include "set.h"
/*
Returns the next word in the input
In lastChar, the character that was after the word (comma, space, etc.) is saved for tracking the other functions */
char* scanWord(Vars* vars){
    char* result;
    char ch;
    int i=0;
    int iteration=0;
    while(i<MAX_WORD_SIZE-1 && iteration<MAX_ITERATIONS) /* Limits the number of iterations to avoid an infinite loop in case of wrong input */
    {
        iteration++;
        ch = getchar();
        /*If should be here comma, is already detected  by 'commaDetected' function, so in any case there should not be a comma in the first character*/
        if(ch==',' && i==0) {
            if (vars->lastChar == ',') {
                puts("Multiple consecutive commas");
            }
            else {
                puts("Illegal comma");
            }
            vars->validInput=0;
            result = NULL;
            break;
        }
        if(ch==' ' || ch=='\t' || ch==',') {
            if(i==0) /*if str yet empty. (not relevant if ch=',')*/
            {
                continue;
            }
            else {
                if(ch==','){
                    if(!vars->commaRequired) { /* Checks if there really should be a comma here */
                        vars->validInput=0;
                        puts("Illegal comma");
                        result = NULL;
                        break;
                    }
                    if(vars->endLine) {
                        vars->validInput=0;
                        puts("Extraneous text after end of command");
                        result = NULL;
                    }
                }
                break;
            }
        }
        if(ch == '\n' || ch == ENTER){
            vars->endLine=1;
            *(vars->str+i)=ch;
            break;
        }
        if(ch == -1){
            vars->isEOF=1;
            vars->validInput=0;
            puts("Error! The program ended without a stop command");
            result = NULL;
            break;
        }
        *(vars->str+i)=ch;
        i++;
    }
    vars->lastChar=ch;
    *(vars->str+i)= '\0'; /* Indicates the end of the string */
    vars->commaRequired=1; /* The starting point is that a comma is required before the next word, unless otherwise indicated */
    if(vars->validInput)
        result = vars->str;
    return result;
}

int scanNumber(Vars *vars)
{
    int result;
    char *strPointer;
    strPointer=scanWord(vars);
    if(strPointer==NULL) /* Case of invalid input */
        result = NULL;
    else
        result = (int)strToInt(strPointer, vars);
    return result;
}
/*
Converts a word to a number
Receives a string and returns its value as an int */
int strToInt(char *strInp, Vars* vars)
{
    int num=0;
    int result=0;
    int i=0;
    int sign = *(strInp+i)=='-' ? -1+(i++): 1;
    char currDigit; /* The  current digit in all each iteration */
    for(; i< strlen(strInp); i++) {
        num*=10; /* We continued scanning the number one digit to the right, so the previous number is multiplied by 10 */
        currDigit = *(strInp+i)-48; /* Shift an ASCII array of the character to a numeric value */
        if(currDigit<0 || 9<currDigit){ /* If this char is not digit */
            puts("Invalid set member - not an integer");
            vars->validInput=0;
            result = NULL;
            break;
        }
        num += currDigit;
    }
    if(vars->validInput) /* Initializes num only if no error was detected on the way */
        result = num*sign;
    return result;
}
/*
Checks if a comma is detected before the next word
Returns 1 if a comma is detected, 0 otherwise */
int commaDetected(Vars* vars)
{
    int result = (int)NULL; /* The value for returning */
    char ch;
    if(vars->lastChar==',')
        result = 1;
    else if(vars->isEOF || vars->endLine)
        result = 0;
    else{
        ch=getchar();
        if(ch==ENTER || ch=='\n' ) vars->endLine = 1; /* For redSet function - case that is
		 last number but is not '-1' */
        if(ch==EOF || ch==-1 ) vars->isEOF = 1;
        while(ch==' ' || ch=='\t') /* skips blanks */
            ch=getchar();
        if(ch==',') {
            vars->lastChar=ch;
            result = 1;
        }
        else if(!vars->isEOF) /* For case that is the last number for redSet function but is not '-1' */
            puts("Missing comma");
    }
    return result;
}
