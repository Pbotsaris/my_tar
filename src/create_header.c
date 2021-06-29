#include "my_tar.h"

/*****************************************************************
 * 
 * 					INITIATION										
 * 
 * **************************************************************/

header_t *init(header_t *header)
{
	memset(header->name, '\0', sizeof(header->name));
	memset(header->mode, '\0', sizeof(header->mode));
	memset(header->uid, '\0', sizeof(header->uid));
	memset(header->gid, '\0', sizeof(header->gid));
	memset(header->size, '\0', sizeof(header->size));
	memset(header->mtime, '\0', sizeof(header->mtime));
	memset(header->chksum, '\0', sizeof(header->chksum));
	memset(header->linkname, '\0', sizeof(header->linkname));
	memset(header->magic, '\0', sizeof(header->magic));
	memset(header->version, '\0', TVERSLEN);
	memset(header->uname, '\0', sizeof(header->uname));
	memset(header->gname, '\0', sizeof(header->gname));
	memset(header->devmajor, '\0', sizeof(header->devmajor));
	memset(header->devminor, '\0', sizeof(header->devminor));
	memset(header->prefix, '\0', sizeof(header->prefix));
	return header;
}

/*!
	
	 HELPER TO: add_typeflag(header_t *header, struct stat stats);
	- add devmajor and devminor when file type is a block or char special
*/
void add_dev_major_minor(header_t *header, struct stat stats)
{
	my_itoa(header->devmajor, (int)major(stats.st_rdev), DECIMAL);
	my_itoa(header->devminor, (int)minor(stats.st_rdev), DECIMAL);
}

/*!
	
	 HELPER TO: add_typeflag(header_t *header, struct stat stats);
	- check if file is either a symlink or regular file.
	- If symlink writes to header->linkname
*/
void add_link_or_regtype(header_t *header, char *path)
{
	struct stat lstats;
	if (lstat(path, &lstats) == 0 && S_IFLNK == (lstats.st_mode & S_IFMT))
	{
		header->typeflag = SYMTYPE;

		size_t buff_size = (lstats.st_size / sizeof(char)) + 1;
		int num_bytes;
		num_bytes = readlink(path, header->linkname, buff_size);
		header->linkname[buff_size + 1] = '\0';
	}
	else
	{
		header->typeflag = REGTYPE;
	}
}

/*!
	
	- Checks file type and wrties to header->typeflag
*/
void add_typeflag(header_t *header, struct stat stats, char *path)
{

	if (S_ISDIR(stats.st_mode))
		header->typeflag = DIRTYPE;
	else if (S_ISREG(stats.st_mode))
		// symlink
		add_link_or_regtype(header, path);

	else if (S_ISCHR(stats.st_mode))
	{
		header->typeflag = CHRTYPE;
		add_dev_major_minor(header, stats);
	}
	else if (S_ISBLK(stats.st_mode))
	{
		header->typeflag = BLKTYPE;
		add_dev_major_minor(header, stats);
	}
	else if (S_ISFIFO(stats.st_mode))
		header->typeflag = FIFOTYPE;
	else if (S_ISLNK(stats.st_mode))
	{
		header->typeflag = LNKTYPE;
	}
	// if not none above
	else
	{
		header->typeflag = REGTYPE;
		printf("%s\n", FLAGTYPE_ERR);
	}
}

/*!
	
	 HELPER TO: add_checksum(header_t *header);
		-  Calculate checksum base on header item size and returns it.
*/
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

/*!
	-  Calculate checksum and writes to  header->chksum
*/
void add_checksum(header_t *header)
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

void add_uid_gid(header_t *header, struct stat stats)
{
	my_itoa(header->uid, stats.st_uid, DECIMAL);
	my_itoa(header->gid, stats.st_gid, DECIMAL);
}

/*!
	- Check OS for relevant mtime field.
	- Writes modified time in seconds to header->mtime
*/
void add_mtime(header_t *header, struct stat stats)
{
	// CHECK OS
#if __APPLE__
	my_itoa(header->mtime, stats.st_mtimespec.tv_sec, DECIMAL);
#elif __linux__
	my_itoa(header->mtime, stats.st_mtim.tv_sec, DECIMAL);
#endif
}

/*!
	- Writes size in bytes to header->size.
*/
void add_size(header_t *header, struct stat stats)
{

	if (stats.st_mode != S_IFLNK)
		my_itoa(header->size, stats.st_size, DECIMAL);
	else
		header->size[0] = '0';
}

/*!
 
	 - Writes user id and group id to header->gname and header->uname respectively.
 
*/
void add_uname_gname(header_t *header, struct stat stats)
{
	struct passwd *pws;
	struct group *grp;
	pws = getpwuid(stats.st_uid);
		grp = getgrgid(stats.st_gid);

	strcpy(header->gname, grp->gr_name);
	strcpy(header->uname, pws->pw_name);
}

/*!
 
	 - read mode, gid, and uid from stat struct and sum them together.
	 - write mode in octal to header->mode
 
*/
void add_mode(header_t *header, struct stat stats)
{
	// ->  TODO:	The mode field provides nine bits specifying file permissions and three bits to specify the Set UID, Set GID, and Save Text (sticky) modes.
	int mode = 0;
	int modes[MODELEN] = {TUREAD, TUWRITE, TUEXEC, TGREAD, TGWRITE, TGEXEC, TOREAD, TOWRITE, TOEXEC};
	int stats_modes[MODELEN] = {S_IREAD, S_IWUSR, S_IXUSR, S_IRGRP, S_IWGRP, S_IXGRP, S_IROTH, S_IWOTH, S_IXOTH};

	for (int i = 0; i < MODELEN; ++i)
		if (stats.st_mode & stats_modes[i])

			mode += stats_modes[i];

	my_itoa(header->mode, mode, OCTAL);
}

/*!
 
	 - Writes filename to be archieved to header->name
 
*/
void add_name(header_t *header, char *path)
{
	size_t path_len = strlen(path);
	if (path_len < NAMELEN)
	{

		strcpy(header->name, path);
		header->prefix[0] = '\0';
	}

	else if (path_len < NAMELEN * 2)
	{
		int split_pos = path_len - NAMELEN + 1;
		strncpy(header->prefix, path, split_pos);
		header->prefix[split_pos] = '\0';
		strncpy(header->name, &path[split_pos], NAMELEN);
		header->name[NAMELEN - 1] = '\0';
	}
	else
		printf("%s", EXC_NAME_SIZE);
}

void add_magic_version(header_t *header)
{
		strncpy(header->magic, TMAGIC, TMAGLEN);
		header->magic[TMAGLEN - 1] = '\0';
		strncpy(header->version, TVERSION, TVERSLEN);
	  header->version[TVERSLEN - 1] = '\0';	
}

/********************************************/ /****************************************************************
 *  Create Header																								*																									*
 *  																											*																												* 
 *   - Using the path passed in as first argument, create_header creates a tar struct following the				*				
 *  	 GNU basic tar convention: https://www.gnu.org/software/tar/manual/html_node/Standard.html				*					
 *   - This header struct is based on the Tar Header Block, from POSIX 1003.1-1990.								*							
 *   - This header struct adds a trailing null to every field													*														
 *																												*																												*
 ***************************************************************************************************************/

header_t *create_header(char *path)
{
	header_t *header;
	header = (header_t *)malloc(sizeof(header_t));
	struct stat stats;
	if (stat(path, &stats) == 0)
	{
		header = init(header);
		add_name(header, path);
		add_mtime(header, stats);
		add_mode(header, stats);
		add_typeflag(header, stats, path);
		add_size(header, stats);
	//	add_checksum(header);
		add_magic_version(header);
		add_uid_gid(header, stats);
		add_uname_gname(header, stats);
		printf("%s", TMAGIC);
	}
	else
	{
		printf("%s %s\n", STAT_ERR, path);
	}
	return header;
}
