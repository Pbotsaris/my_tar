#include <stdio.h>

size_t my_strlen(char *str)
{
    int counter = 0;

    while (str[counter])
        counter++;

    return counter;
}

sad