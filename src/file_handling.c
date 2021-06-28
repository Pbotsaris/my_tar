#include "my_tar.h"

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

void file_info(header_t *header, struct stat stats)
{
	/* [> User ID of owner <] */
	my_itoa(header->uid, stats.st_uid, DECIMAL);

	/* [> Group ID of owner <] */
	my_itoa(header->gid, stats.st_gid, DECIMAL);

	/* [> Total size, in bytes <] */
	my_itoa(header->size, stats.st_size, DECIMAL);

	/* [> Number of hard links <] */
	my_itoa(header->linkname, stats.st_nlink, DECIMAL);
}


void add_dev_major_minor(header_t *header, struct  stat stats)
{
			  my_itoa(header->devmajor, (int)major(stats.st_rdev), DECIMAL);
			  my_itoa(header->devminor, (int)minor(stats.st_rdev), DECIMAL);
}


void add_filetype(header_t *header, struct stat stats)
{

	if(S_ISDIR(stats.st_mode))
		 header->typeflag = DIRTYPE; 
	else if(S_ISREG(stats.st_mode))
		header->typeflag = REGTYPE;
	else if(S_ISCHR(stats.st_mode)){
		header->typeflag =CHRTYPE;
		add_dev_major_minor(header, stats);
	}
	else if(S_ISBLK(stats.st_mode)){
		header->typeflag = BLKTYPE;
		add_dev_major_minor(header, stats);
	}
	else if(S_ISFIFO(stats.st_mode))
		header->typeflag = FIFOTYPE;
	else if(S_ISLNK(stats.st_mode))
		header->typeflag = SYMTYPE;
	// if not none above
	else{
		header->typeflag = REGTYPE;
		printf("%s\n", FLAGTYPE_ERR);
	}
		

}

void add_mode(header_t *header, struct stat stats)
{
// ->  TODO:	The mode field provides nine bits specifying file permissions and three bits to specify the Set UID, Set GID, and Save Text (sticky) modes.
	int mode = 0;

	int modes[NUM_MODES] = {TUREAD, TUWRITE, TUEXEC, TGREAD, TGWRITE, TGEXEC, TOREAD, TOWRITE, TOEXEC};
	int stats_modes[NUM_MODES] = {S_IREAD, S_IWUSR, S_IXUSR, S_IRGRP, S_IWGRP, S_IXGRP, S_IROTH, S_IWOTH, S_IXOTH};

	for (int i = 0; i < NUM_MODES; ++i)
		if (stats.st_mode & stats_modes[i])
			mode = mode + modes[i];

	my_itoa(header->mode, mode, OCTAL);
}

void add_name(header_t *header, char *path)
{
  size_t path_len = strlen(path);
  if(path_len < MAX_NAME_SIZE){
		strcpy(header->name, path);
		header->prefix[0] = '\0';
	}

	else if(path_len < MAX_NAME_SIZE * 2){ 
		int split_pos = path_len - MAX_NAME_SIZE + 1; 
		printf("split pos %i\n", split_pos);
		strncpy(header->prefix, path, split_pos);
		header->prefix[split_pos] = '\0';
		strncpy(header->name, &path[split_pos], MAX_NAME_SIZE);
		header->name[MAX_NAME_SIZE - 1] = '\0';
		}
	 else
		 printf("%s", EXC_NAME_SIZE);

}

header_t *create_header(char *path)
{
	header_t *header;
	header = (header_t *)malloc(sizeof(header_t));

	struct stat stats;
	if (stat(path, &stats) == 0)
	{

    add_name(header, path);
		add_mode(header, stats);
		add_filetype(header, stats);
		file_info(header, stats);
		
	}
	else
	{
		printf("%s %s\n", STAT_ERR, path);
	}
	return header;
}
