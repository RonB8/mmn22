//
// Created by user on 07/11/2022.
//
#include <stdio.h>

int isOrder(char x, char y, char z)
{
    return (x+1 == y && y+1 == z);
}

void f(char *str, int start, int end)
{
    *(str+2) = '\0';
    *(str+3) = 'g';
}

void shortStr(char *str)
{
    int i, writer=0;
    char temp = &(*(str+i));
    char before, current, after;
    for(i=1; *(str+i+1) != '\0'; i++) {
        if (isOrder(temp, (str + i), (str+i+1))) {
            temp = &(*(str+i));
            *(str + i) = '-';
        }
    }
    char *s, *e;
    for(i=1; (str+i+1) != '\0'; i++)
    {
        if (*(str+i-1)=='-' && *(str+i)=='-')
        {
            *s = &(*(str+i));
        }

    }
    }
    }



//before = *(str+i-1);
//current = *(str+i);
//after = *(str+i+1);
