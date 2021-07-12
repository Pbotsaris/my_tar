#include "../include/messages.h"
#include "../include/my_tar.h"
#include "../include/header.h"

/*
 *

        - ITOA HELPER: reverses a sequence of digits in char*

*/

void reverse(char str[], int length) {
  char temp_str[length];
  for (int i = 0; i < length; ++i)
    temp_str[i] = str[i];

  int end = length - 1;
  for (int i = 0; i < length; ++i) {
    str[i] = temp_str[end];
    end--;
  }
}

/*
 *

        -  converts from integer digits to ASCII char digits
  ->  returns the length of the char*

*/

int my_itoa(char *str, int num, int base) {
  int idx = 0;
  if (num == 0) {
    str[idx] = '0';
    return 1;
  }

  while (num != 0) {
    int remainder = num % base;
    // remainder returns leftmost digits - 9 % 10 = 9  19 % 10 = 9
    str[idx++] = (remainder > 9) ? (char)((remainder - 10) + 'a')
                                 : (char)(remainder + '0');
    num = num / base;
  }

  reverse(str, idx);
  //    str[idx] = '\0';
  return idx;
}

/*
 *

        -> returns an array of byte position for each field in the header struct

*/

int *create_bytes_offset(void) {
  int b[BYTOFFLEN] = {
      0,        NAMELEN,   MODELEN,     UIDLEN,      GIDLEN,   SIZELEN,
      MTIMELEN, CHKSUMLEN, LINKNAMELEN, TYPFLAGLEN,  TMAGLEN,  TVERSLEN,
      UNAMELEN, GNAMELEN,  DEVMAJORLEN, DEVMINORLEN, PREFIXLEN};
  int *bytes_offset = (int *)malloc(BYTOFFLEN * sizeof(int));

  for (int i = 0; i < BYTOFFLEN; ++i)
    bytes_offset[i] = b[i];

  return bytes_offset;
}

/*
 *

        -> returns an array of mode values in octal.

*/

int *create_modes(modes_t type) {
  int *modes = (int *)malloc(MODES_ARR_LEN * sizeof(int));

  int tar_modes[MODES_ARR_LEN] = {TUREAD, TUWRITE, TUEXEC,  TGREAD, TGWRITE,
                                  TGEXEC, TOREAD,  TOWRITE, TOEXEC};

  int stat_modes[MODES_ARR_LEN] = {S_IREAD, S_IWUSR, S_IXUSR, S_IRGRP, S_IWGRP,
                                   S_IXGRP, S_IROTH, S_IWOTH, S_IXOTH};

  for (int i = 0; i < MODES_ARR_LEN; ++i)
    modes[i] = type == tar_mode ? tar_modes[i] : stat_modes[i];

  return modes;
}

/*
 *

        -  Fills a buffer with 0 for unused indexes
        -  returns -1 on error

*/

int fill_zeros(char *field, int len, int total_len) {
  if (total_len < (int)strlen(field))
    return 1;

  int j = len;
  char buff[total_len];
  memset(buff, '0', total_len - 1);

  for (int i = 0; i < len; i++) {
    buff[(total_len - 1) - j] = field[i];
    j--;
  }
  buff[total_len - 1] = '\0';

  strcpy(field, buff);

  return 0;
}

/*
 *
           - converts a decimal number to octal base
     -> returns octal number

*/

int decimal_to_octal(int decimal) {
  int octal = 0;
  int num_digits = 1;
  int temp = 0;
  temp = decimal;
  while (temp != 0) {

    octal += (temp % 8) * num_digits;
    temp /= 8;
    num_digits *= 10;
  }
  return octal;
}

/*
 *
    -memsets
 */

void *my_memset(void *str, int c, int len) {
  unsigned char *p = str;
  while (len--) {
    *p++ = (unsigned char)c;
  }
  return str;
}
