#include "my_tar.h"

unsigned int checksum_calculator(char *header, size_t size)
{
	int index = 0,
		check = 0;

	while (header[index] && index < size)
	{
		check += header[index];
		index++;
	}
	return check;
}


bool_t symlink_exists(const char* path)
{
    struct stat stats;
    int result;
    result = lstat(path, &stats);
    return (result == 0);
}


void checksum(header_t *header)
{
	unsigned int check = 0;

	check += checksum_calculator(header->name, 100);
	check += checksum_calculator(header->mode, 10);
	check += checksum_calculator(header->uid, 8);
	check += checksum_calculator(header->gid, 8);
	check += checksum_calculator(header->size, 12);
	check += checksum_calculator(header->mtime, 12);
	check += header->typeflag;
	check += 8 * 32;
	check += checksum_calculator(header->linkname, 100);
	check += checksum_calculator(header->version, 2);
	check += checksum_calculator(header->uname, 32);
	check += checksum_calculator(header->gname, 32);
	check += checksum_calculator(header->prefix, 155);
	sprintf(header->chksum, "%0*o ", 8, check);
}

void file_info(header_t *header, struct stat stats)
{

	/* [> User ID of owner <] */
	my_itoa(header->uid, stats.st_uid, DECIMAL);

	/* [> Group ID of owner <] */
	my_itoa(header->gid, stats.st_gid, DECIMAL);

	/* [> Total size, in bytes <] */
	if (stats.st_mode != S_IFLNK)
		my_itoa(header->size, stats.st_size, DECIMAL);
	else
		header->size[0] = '0';

	/* [> Modified time in seconds <] */
//	my_itoa(header->mtime, stats.st_mtim.tv_sec, DECIMAL);

	/* [> Check Sum <] */
	//checksum(header);
}


void add_dev_major_minor(header_t *header, struct stat stats)
{
			  my_itoa(header->devmajor, (int)major(stats.st_rdev), DECIMAL);
			  my_itoa(header->devminor, (int)minor(stats.st_rdev), DECIMAL);
}


void add_filetype(header_t *header, struct stat stats, char * path)
{

	if(S_ISDIR(stats.st_mode))
		 header->typeflag = DIRTYPE; 
	else if(S_ISREG(stats.st_mode)){
		if(symlink_exists(path) == TRUE)
			header->typeflag =  SYMTYPE;
		else
			header->typeflag = REGTYPE;
	}
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
	else if(S_ISLNK(stats.st_mode)){
		header->typeflag =  LNKTYPE;
	}
	// if not none above
	else{
					header->typeflag = REGTYPE;
			printf("%s\n", FLAGTYPE_ERR);
	}
		
}

void add_uname_gname(header_t *header, struct stat stats)
{
	struct passwd *pws;
	struct group *grp;
	pws = getpwuid(stats.st_uid);
	grp = getgrgid(stats.st_gid);

	strcpy(header->gname, grp->gr_name);
	strcpy(header->uname, pws->pw_name);
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
		add_filetype(header, stats, path);
		add_uname_gname(header, stats);
		file_info(header, stats);
	}
	else
	{
		printf("%s %s\n", STAT_ERR, path);
	}
	return header;
}
