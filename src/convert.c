#include "my_tar.h"

/*!
	- HELPER: converts a decimal number to octal base
*/

int decimal_to_octal(int decimal)
{
    int octal = 0;
    int num_digits = 1;
    int temp = decimal;
    while (temp != 0)
    {

        octal += (temp % 8) * num_digits;
        temp /= 8;
        num_digits *= 10;
    }
    return octal;
}

void reverse(char str[], int length)
{
    char temp_str[length];
    for (int i = 0; i < length; ++i)
        temp_str[i] = str[i];

    int end = length - 1;
    for (int i = 0; i < length; ++i)
    {
        str[i] = temp_str[end];
        end--;
    }
}

int my_itoa(char *str, int num, int base)
{
    int idx = 0;
    if (num == 0)
    {
        str[idx] = '0';
        return 1;
    }

    while (num != 0)
    {
        int remainder = num % base;
        // remainder returns leftmost digits - 9 % 10 = 9  19 % 10 = 9
        str[idx++] = (remainder > 9) ? (char)((remainder - 10) + 'a') : (char)(remainder + '0');
        num = num / base;
    }

    reverse(str, idx);
    //    str[idx] = '\0';

    return idx;
}