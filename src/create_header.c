#include "my_tar.h"
#define MODES_ARR_LEN 9

/*!
	- HELPER: converts a decimal number to octal base
*/

int decimal_to_octal(int decimal)
{
	int octal = 0;
	int num_digits = 1;
	int temp = decimal;
	while (temp != 0)
	{

		octal += (temp % 8) * num_digits;
		temp /= 8;
		num_digits *= 10;
	}
	return octal;
}

/*!
	- HELPER: Fills a buffer with 0 for unused indexes
*/

void fill_zeros(char *field, int len, int total_len)
{
	int j = len;
	for (int i = 0; i < len; i++)
	{
		field[total_len - 1 - j] = field[i];
		j--;
	}
	memset(field, '0', total_len - len - 1);
	field[total_len - 1] = '\0';
}

/*!
	
	 HELPER TO: add_typeflag(header_t *header, struct stat stats);
	- add devmajor and devminor when file type is a block or char special
*/

void add_dev_major_minor(header_t *header, struct stat stats)
{
	int devmajor = (int)major(stats.st_rdev);
	int len = my_itoa(header->devmajor, devmajor, OCTAL);
	fill_zeros(header->devmajor, len, DEVMAJORLEN);
	int devminor = (int)minor(stats.st_rdev);
	len = my_itoa(header->devminor, devminor, OCTAL);
	fill_zeros(header->devminor, len, DEVMINORLEN);
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

unsigned int checksum8b(char *field, size_t len)
{
//	size_t len = strlen(field);
	unsigned int sum = 0;
	for(int i = 0; i < len; i++)
		++sum; 

	return sum;

}

/*!
	-  Calculate checksum and writes to  header->chksum
*/
void add_checksum(header_t *header)
{
	unsigned int chksum = 0;
 	chksum += checksum8b(header->name, NAMELEN);
 	chksum += checksum8b(header->mode, MODELEN);
 	chksum += checksum8b(header->uid, UIDLEN);
 	chksum += checksum8b(header->gid, GIDLEN);
 	chksum += checksum8b(header->size, SIZELEN);
 	chksum += checksum8b(header->mtime, MTIMELEN);
 	chksum += checksum8b(header->linkname, LINKNAMELEN);
 	chksum += checksum8b(header->magic, TMAGLEN);
 	chksum += checksum8b(header->version, TVERSLEN);
 	chksum += checksum8b(header->uname, UNAMELEN);
 	chksum += checksum8b(header->gname, GNAMELEN);
 	chksum += checksum8b(header->devmajor, DEVMAJORLEN);
 	chksum += checksum8b(header->devmajor, DEVMAJORLEN);
 	chksum += checksum8b(header->prefix, PREFIXLEN);
 	chksum += checksum8b(header->prefix, PREFIXLEN);
	chksum += 1;

	// TODO: checksum change from 1073 to 1071 when fill with zeros
	int len = my_itoa(header->chksum, decimal_to_octal(chksum), DECIMAL);
	header->chksum[CHKSUMLEN -1] = '\0';
//	fill_zeros(header->chksum, len, CHKSUMLEN);
	

	/*chksum += sizeof(header->name);*/
	/*chksum += sizeof(header->mode);*/
	/*chksum += sizeof(header->uid);*/
	/*chksum += sizeof(header->gid);*/
	/*chksum += sizeof(header->size);*/
	/*chksum += sizeof(header->mtime);*/
	/*chksum += sizeof(header->typeflag);*/
	/*chksum += sizeof(header->version);*/

	/*chksum += sizeof(header->magic);*/
	/*chksum += sizeof(header->uname);*/
	/*chksum += sizeof(header->gname);*/
	/*chksum += sizeof(header->prefix);*/

	/*// optional*/
	/*if (header->linkname[0] != '\0')*/
		/*chksum += sizeof(header->linkname);*/

	/*if (header->devmajor[0] != '\0')*/
	/*{*/
		/*chksum += sizeof(header->devmajor);*/
		/*chksum += sizeof(header->devmajor);*/
	/*}*/

}
//f.txt0000644 0000765 00000240000000005114067333751011045 0ustar  pedrostafftrying tar for khalil. what will happen?

void add_uid_gid(header_t *header, struct stat stats)
{
	int len = my_itoa(header->uid, stats.st_uid, OCTAL);

	fill_zeros(header->uid, len, UIDLEN);

	len = my_itoa(header->gid, stats.st_gid, OCTAL);
	fill_zeros(header->gid, len, GIDLEN);
}

/*!
	- Check OS for relevant mtime field.
	- Writes modified time in seconds to header->mtime
*/
void add_mtime(header_t *header, struct stat stats)
{
	// CHECK OS
	int len;
#if __APPLE__
			len = my_itoa(header->mtime, stats.st_mtimespec.tv_sec, OCTAL);
			fill_zeros(header->mtime, len, MTIMELEN);
#elif __linux__
			len = my_itoa(header->mtime, stats.st_mtim.tv_sec, OCTAL);
			fill_zeros(header->mtime, len, MTIMELEN);
#endif
}

/*!
	- Writes size in bytes to header->size.
*/
void add_size(header_t *header, struct stat stats)
{

	if (stats.st_mode != S_IFLNK)
	{

		int len = my_itoa(header->size,stats.st_size, OCTAL);


		fill_zeros(header->size, len, SIZELEN);
	}
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
	int modes[MODES_ARR_LEN] = {TUREAD, TUWRITE, TUEXEC, TGREAD, TGWRITE, TGEXEC, TOREAD, TOWRITE, TOEXEC};
	int stats_modes[MODES_ARR_LEN] = {S_IREAD, S_IWUSR, S_IXUSR, S_IRGRP, S_IWGRP, S_IXGRP, S_IROTH, S_IWOTH, S_IXOTH};

	for (int i = 0; i < MODES_ARR_LEN; ++i)
		if (stats.st_mode & stats_modes[i])

			mode += stats_modes[i];

	int len = my_itoa(header->mode, mode, OCTAL);
	fill_zeros(header->mode, len, MODELEN);
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

	for(int i = 0; i < TVERSLEN; ++i)
			header->version[i] = ' ';

	header->version[TVERSLEN -1] = '\0';	
}

void init_optional_fields(header_t *header)
{
	header->devmajor[0] = '\0';
	header->devminor[0] = '\0';
	header->linkname[0] = ' ';
	header->linkname[1] = '\0';
	header->prefix[0] = '\0';
}

/********************************************/ /****************************************************************
 *  Create Header																								*																									*
 *  																											*																												* 
 *   - Using the path passed in as first argument, create_header creates a tar struct following the				*				
  basic tar convention: https://www.gnu.org/software/tar/manual/html_node/Standard.html				*					
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
		init_optional_fields(header);
		add_name(header, path);
		add_mtime(header, stats);
		add_mode(header, stats);
		add_typeflag(header, stats, path);
		add_size(header, stats);

		add_magic_version(header);
		add_uid_gid(header, stats);
		add_uname_gname(header, stats);

		add_checksum(header);

	}
	else
	{
		printf("%s %s\n", STAT_ERR, path);
	}
	return header;
}
