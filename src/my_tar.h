/* ========================================================================= */

// LIBRARIES

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <unistd.h>
#include <string.h>
//#include <sys/sysmacros.h>
#include <ctype.h>
#include <fcntl.h>

/* ========================================================================= */

#ifndef MY_TAR_H
#define MY_TAR_H

#define OCTAL 8
#define DECIMAL 8

#define TMAGIC "ustar" /* ustar and a null */
#define TVERSION "00"  /* 00 and no null */

// VALUES IN OCTAL
#define TUREAD 00401  /* read by owner */
#define TUWRITE 00202 /* write by owner */
#define TUEXEC 00100  /* execute/search by owner */
#define TGREAD 00040  /* read by group */
#define TGWRITE 00020 /* write by group */
#define TGEXEC 00010  /* execute/search by group */
#define TOREAD 00004  /* read by other */
#define TOWRITE 00002 /* write by other */
#define TOEXEC 00001  /* execute/search by other */

/* typeflag field. */
#define REGTYPE '0'   /* regular file */
#define AREGTYPE '\0' /* regular file */
#define LNKTYPE '1'   /* link */
#define SYMTYPE '2'   /* reserved */
#define CHRTYPE '3'   /* character special */
#define BLKTYPE '4'   /* block special */
#define DIRTYPE '5'   /* directory */
#define FIFOTYPE '6'  /* FIFO special */
#define CONTTYPE '7'  /* reserved */

#define NAMELEN 100
#define MODELEN 8
#define UIDLEN 8
#define GIDLEN 8
#define SIZELEN 12
#define MTIMELEN 12
#define CHKSUMLEN 8
#define LINKNAMELEN 100
#define TMAGLEN 6
#define UNAMELEN 32
#define GNAMELEN 32
#define TVERSLEN 3
#define DEVMAJORLEN 8
#define DEVMINORLEN 8
#define PREFIXLEN 155

// The name, linkname, magic, uname, and gname are null-terminated
// All other fields are zero-filled octal numbers in ASCII

typedef struct posix_header
{                     /* byte offset */
  char name[NAMELEN]; /*   0 */
  char mode[MODELEN];
  char uid[UIDLEN];           /* 108 */
  char gid[GIDLEN];           /* 116 */
  char size[SIZELEN];         /* 124 */
  char mtime[MTIMELEN];       /* 136 */
  char chksum[CHKSUMLEN];     /* 148 */
  char typeflag;              /* 156 */
  char linkname[LINKNAMELEN]; /* 157 */
  char magic[TMAGLEN];        /* 257 */
  char version[TVERSLEN];     /* 263 */
  char uname[UNAMELEN];       /* 265 */
  char gname[GNAMELEN];       /* 297 */
  char devmajor[DEVMAJORLEN]; /* 329 */
  char devminor[DEVMINORLEN]; /* 337 */
  char prefix[PREFIXLEN];     /* 345 */
                              /* 500 */
} header_t;

header_t *create_header(char *path);
void archive(char *path, char **argv, int argc);
int ls_tar(char *path);

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
  FALSE,
  TRUE,
  ERRORF
} bool_t;

// ERRS
#define F_NOT_FOUND "my_tar: Refusing to read archive contents from terminal (missing -f option?)\n"
#define F_ERROR "You must specify one of the the following options -c -r -t -u -x\n"
#define NULL_OPT "my_tar: Error is not recoverable: exiting now\n"
#define EXC_NAME_SIZE "my_tar: Filename exceeds maximum length of 200\n"
#define STAT_ERR "Unable to read"
#define FLAGTYPE_ERR "File type not recognized. Setting as regular file."

option_t check_option(char **format);

#endif

/* ========================================================================= */

#ifndef MY_ITOA_H
#define MY_ITOA_H

int my_itoa(char *str, int num, int base);
int my_atoi(char *str);

#endif
