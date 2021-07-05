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
#if __linux__
#include <sys/sysmacros.h>
#endif
#include <fcntl.h>

/* ========================================================================= */

#ifndef MY_TAR_H
#define MY_TAR_H

#define TMAGIC "ustar" /* ustar and a null */
#define TVERSION "00"  /* 00 and no null */

/* USERS - values in octal */
#define TSUID 04000 /* set UID on execution */
#define TSGID 02000 /* set GID on execution */
#define TSVTX 01000 /* reserved */


/* MODES - values in octal */
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

/* field length */
#define NAMELEN 100
#define MODELEN 8
#define UIDLEN 8
#define GIDLEN 8
#define SIZELEN 12
#define MTIMELEN 12
#define CHKSUMLEN 8
#define LINKNAMELEN 100
#define TYPFLAGLEN 1
#define TMAGLEN 6
#define UNAMELEN 32
#define GNAMELEN 32
#define TVERSLEN 2
#define DEVMAJORLEN 8
#define DEVMINORLEN 8
#define PREFIXLEN 155

/* Other macros  */
#define OCTAL 8
#define DECIMAL 10
#define BLOCKSIZE 512
#define BYTOFFLEN 17
#define JMPSIZE 24
#define ENDBLK 376

// The name, linkname, magic, uname, and gname are null-terminated
// All other fields are zero-filled octal numbers in ASCII
typedef struct posix_header
{                             /* byte offset */
  char name[NAMELEN];         /*   0 */
  char mode[MODELEN];         /* 100 */
  char uid[UIDLEN];           /* 108 */
  char gid[GIDLEN];           /* 116 */
  char size[SIZELEN];         /* 124 */
  char mtime[MTIMELEN];       /* 136 */
  char chksum[CHKSUMLEN];     /* 148 */
  char linkname[LINKNAMELEN]; /* 156 */
  char typeflag;              /* 256 */
  char magic[TMAGLEN];        /* 257 */
  char version[TVERSLEN];     /* 263 */
  char uname[UNAMELEN];       /* 265 */
  char gname[GNAMELEN];       /* 297 */
  char devmajor[DEVMAJORLEN]; /* 329 */
  char devminor[DEVMINORLEN]; /* 337 */
  char prefix[PREFIXLEN];     /* 345 */
                              /* 500 */
} header_t;


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

typedef enum {tar_mode, stat_mode}modes_t;

// ERRS
#define MISSING_F_ERR "my_tar: Refusing to read archive contents from terminal (missing -f option?)\n"
#define MISSING_OPT_ERR "You must specify one of the the following options -c -r -t -u -x\n"
#define NULL_OPT "my_tar: Error is not recoverable: exiting now\n"
#define EXC_NAME_SIZE "my_tar: Filename exceeds maximum length of 200\n"
#define STAT_ERR "Unable to read"
#define FLAGTYPE_ERR "File type not recognized. Setting as regular file."


#define MODES_ARR_LEN 9


// Helpers
int *create_bytes_offset(void);
int *create_modes(modes_t type);
void fill_zeros(char *field, int len, int total_len) ;
int my_itoa(char *str, int num, int base);
int decimal_to_octal(int decimal);
int my_ls_tar(char *path);
int check_byte(int block);
void debug_header(header_t *header);
void *my_memset(void* str, int c, int len);

// main calbacks
header_t *create_header(char *path, struct stat stats);
int archive(char **paths, size_t paths_len, header_t *headers[]);
option_t check_option(char **format);

bool_t search_flag(char **argv, char flag);
int find_paths_start_index(char **argv);
bool_t validate_tar_extention(char *path);

#endif
