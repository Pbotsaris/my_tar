#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include "my_tar.h"
#include "my_itoa.h"
#include "option.h"

#define STAT_ERR "Unable to read"

/*dev_t     st_dev;         [> ID of device containing file <]*/
/*ino_t     st_ino;         [> Inode number <]*/
/*mode_t    st_mode;        [> File type and mode <]*/
/*nlink_t   st_nlink;       [> Number of hard links <]*/
/*uid_t     st_uid;         [> User ID of owner <]*/
/*gid_t     st_gid;         [> Group ID of owner <]*/
/*dev_t     st_rdev;        [> Device ID (if special file) <]*/
/*off_t     st_size;        [> Total size, in bytes <]*/
/*blksize_t st_blksize;     [> Block size for filesystem I/O <]*/
/*blkcnt_t  st_blocks;      [> Number of 512B blocks allocated <]*/

// S_ISDIR(stats.st_mode);

void add_mode(header_t *header, struct stat stats)
{

	int mode = 0;

	int modes[NUM_MODES] = {TUREAD, TUWRITE, TUEXEC, TGREAD, TGWRITE, TGEXEC, TOREAD, TOWRITE, TOEXEC};
	int stats_modes[NUM_MODES] = {S_IREAD, S_IWUSR, S_IXUSR, S_IRGRP, S_IWGRP, S_IXGRP, S_IROTH, S_IWOTH, S_IXOTH};

	for (int i = 0; i < NUM_MODES; ++i)
		if (stats.st_mode & stats_modes[i])
			mode = mode + modes[i];

	my_itoa(header->mode, mode, OCTAL);
}

header_t *create_header(char *path)
{
	//char path[] = "text.txt";
	header_t *header;
	header = (header_t *)malloc(sizeof(header_t));

	struct stat stats;
	if (stat(path, &stats) == 0)
	{
		strcpy(header->name, path);
		add_mode(header, stats);
		//	strcpy(header.gid, stats.st_gid);
	}
	else
	{
		printf("%s %s\n", STAT_ERR, path);
	}
	return header;
}
