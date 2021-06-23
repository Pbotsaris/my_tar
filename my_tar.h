/* ========================================================================= */

// LIBRARIES

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

/* ========================================================================= */

#ifndef MY_TAR_H
#define MY_TAR_H

typedef struct posix_header
{                     /* byte offset */
  char name[100];     /*   0 */
  char mode[10];      // changed by PB - 9 modes + 9 char each
  char uid[8];        /* 108 */
  char gid[8];        /* 116 */
  char size[12];      /* 124 */
  char mtime[12];     /* 136 */
  char chksum[8];     /* 148 */
  char typeflag;      /* 156 */
  char linkname[100]; /* 157 */
  char magic[6];      /* 257 */
  char version[2];    /* 263 */
  char uname[32];     /* 265 */
  char gname[32];     /* 297 */
  char devmajor[8];   /* 329 */
  char devminor[8];   /* 337 */
  char prefix[155];   /* 345 */
                      /* 500 */
} header_t;

#define NUM_MODES 9
#define OCTAL 8

// VALUES IN OCTAL
#define TUREAD 00400  /* read by owner */
#define TUWRITE 00200 /* write by owner */
#define TUEXEC 00100  /* execute/search by owner */
#define TGREAD 00040  /* read by group */
#define TGWRITE 00020 /* write by group */
#define TGEXEC 00010  /* execute/search by group */
#define TOREAD 00004  /* read by other */
#define TOWRITE 00002 /* write by other */
#define TOEXEC 00001  /* execute/search by other */

#define NA 00000 /* 0 if non applicable */

header_t *create_header(char *path);

#endif

/* ========================================================================= */

#ifndef OPTION_H
#define OPTION_H

typedef enum
{
  c,
  r,
  t,
  u,
  x,
  NONE,
  ERROROPT
} option_t;

typedef enum
{
  TRUE,
  FALSE,
  ERRORF
} bool_f;

#define F_NOT_FOUND "my_tar: Refusing to read archive contents from terminal (missing -f option?)\n"
#define F_ERROR "You must specify one of the the following options -c -r -t -u -x\n"
#define NULL_OPT "my_tar: Error is not recoverable: exiting now\n"

option_t check_option(char **format);

#endif

/* ========================================================================= */

#ifndef MY_ITOA_H
#define MY_ITOA_H

void my_itoa(char *str, int num, int base);

#endif
