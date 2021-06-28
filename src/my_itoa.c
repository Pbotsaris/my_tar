#include "my_tar.h"

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

void my_itoa(char *str, int num, int base)
{
    int idx = 0;
    if (num == 0)
    {
        str[idx] = '0';
        return;

    }

    while (num != 0)
    {
        int remainder = num % base;
        // remainder returns leftmost digits - 9 % 10 = 9  19 % 10 = 9
        str[idx++] = (remainder > 9) ? (char)((remainder - 10) + 'a') : (char)(remainder + '0');
        num = num / base;
    }

    reverse(str, idx);
    str[idx] = '\0';
}
