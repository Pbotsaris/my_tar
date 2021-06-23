#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include "my_tar.h"

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


//printf("File Permissions: \t");
//    printf( (S_ISDIR(fileStat.st_mode)) ? "d" : "-");

void add_mode(header_t *header, struct stat stats)
{
itoa(header->mode[0], TUREAD, 8);

header->mode[0] =	stats.st_mode & S_IRUSR ? TUREAD : NA;
header->mode[1] =	stats.st_mode & S_IWUSR ? TUWRITE : NA;
header->mode[2] =	stats.st_mode & S_IXUSR ? TUEXEC : NA;
header->mode[3] =	stats.st_mode & S_IRGRP ? TGREAD : NA;
header->mode[4] =	stats.st_mode & S_IWGRP ? TGWRITE : NA;
header->mode[5] =	stats.st_mode & S_IXGRP ? TGEXEC  : NA ;
header->mode[6] =	stats.st_mode & S_IROTH ? TOREAD : NA ;
header->mode[7] =	stats.st_mode & S_IWOTH ? TOWRITE : NA ;
header->mode[8] =	stats.st_mode & S_IXOTH ? TOEXEC : NA ;

}



int  main(void)
{
		FILE *file;
		file = fopen("test.txt", "r");
		if(file == NULL)	
				return 1;

		char path[] = "text.txt";
     
		header_t header;
		struct stat stats;
    if(stat(path, &stats) == 0){
		strcpy(header.name, path);
		add_mode(&header, stats);
		strcpy(header.gid, stats.st_gid);

    printf("%u\n", stats.st_mode);
		

		} else {
				printf("%s %s\n", STAT_ERR, path);
		}


		return 0;
}


