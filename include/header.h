#ifndef HEADER_H
#define HEADER_H

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
#define ENDBLK 183
#define HEADERBYTE 12
#define JMPTIME 36
#define ENDHEADER 376

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

header_t *get_header(int tar);
header_t *create_header(char *path, struct stat stats);
int next_header_position(header_t *header);

#endif
