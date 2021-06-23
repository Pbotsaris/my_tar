#include "my_itoa.h"
#include <stdio.h>

void reverse(char str[], int length)
{
     char temp_str[length];
    for(int i = 0; i < length; ++i)
        temp_str[i] = str[i];

    int end = length - 1;
    for(int i = 0; i < length; ++i){
       str[i] = temp_str[end];
         end--;
    }
}

void my_itoa(char* str, int num, int base)
{
		printf("num going in %i\n", num);
    int idx = 0;
    bool_t isNeg = FALSE;
    if (num == 0){
        str[idx] = '0';
        return ;
    }
    // negative with base 10 only
    if (num < 0 && base == 10){
        isNeg = TRUE;
        num = -num;
    }

    while (num != 0){
        int remainder = num % base;
        // remainder returns leftmost digits - 9 % 10 = 9  19 % 10 = 9
        str[idx++] = (remainder > 9) ? (char)((remainder - 10) + 'a') : (char)(remainder + '0');
        num = num/base;
    }

		    if (isNeg) 
				     str[idx++] = '-';
    

    reverse(str, idx);
    str[idx] = '\0';
}

