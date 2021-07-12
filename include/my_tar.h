#ifndef MY_TAR_H
#define MY_TAR_H

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#if __linux__
#include <sys/sysmacros.h>
#endif
#include <fcntl.h>

#define MODES_ARR_LEN 9

typedef enum
{
  c,
  r,
  t,
  u,
  x,
  d,
  NONE,
  MISSING_F,
  ERROROPT
} option_t;

typedef enum
{
  FALSE,
  TRUE,
} bool_t;

typedef enum
{
  tar_mode,
  stat_mode
} modes_t;

/*       helpers       */
int *create_bytes_offset(void);
int *create_modes(modes_t type);
int fill_zeros(char *field, int len, int total_len);
int my_itoa(char *str, int num, int base);
int decimal_to_octal(int decimal);
int octal_to_decimal(int octal);
int check_byte(int block);
int debug_header(char *path);
void *my_memset(void *str, int c, int len);

/*       archive       */
int archive(char **paths, size_t paths_len, option_t option);

/*      options       */
option_t check_option(char **format);
int search_flag(char **argv, char flag);
int find_paths_start_index(char **argv);
bool_t validate_tar_extention(char *path);

/*      list       */
int list_or_extract(char *path, option_t options);

#endif
