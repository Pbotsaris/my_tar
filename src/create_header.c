#include "my_tar.h"

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

/*
 *
	
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

/*
 * 
 
	- Checks file type and wrties tomy_itoa header->typeflag


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
	// if not none above
	else
	{
		header->typeflag = REGTYPE;
		printf("%s\n", FLAGTYPE_ERR);
	}
}

/*
 * 
 
	-  HELPER: Calculate checksum of a field in the header-struct

																												*/


unsigned int checksum(char *field, size_t len)
{
	char *temp = field;
	unsigned int sum = 0;
	for (int i = len - 1; i >= 0; i--){
		sum += (unsigned char)*temp;
		temp++;
	}
	return sum;
}

/*
 *
 
	-  Calculate checksum and writes to  header->chksum

																											*/

void add_checksum(header_t *header)
{

	int *bytes_offset = create_bytes_offset();

	unsigned int chksum = 0;
	char *temp = header->name;
	int i;

	for (i = 0; i < BYTOFFLEN - 1; ++i) {
			temp += bytes_offset[i];
			chksum += checksum(temp, bytes_offset[i + 1]);
	}

	/*	 remove chksum field from calculation   */
	for (i = sizeof(header->chksum); i-- != 0;)
		chksum -= (unsigned char) header->chksum[i];
	chksum += ' ' * sizeof header->chksum;

	int len =	my_itoa(header->chksum, decimal_to_octal(chksum), OCTAL);
	fill_zeros(header->chksum, len, CHKSUMLEN);
	// remove extra 0 from chksum
	
		for(int i = 0; i < - CHKSUMLEN; i++)
		{
		if(i == 0)
			continue;

		header->chksum[i] = header->chksum[i + 1] ;

		}
	header->chksum[CHKSUMLEN - 2] = ' ';
	header->chksum[CHKSUMLEN - 1] = '\0';

	free(bytes_offset);


}

void add_uid_gid(header_t *header, struct stat stats)
{
	int len = my_itoa(header->uid, stats.st_uid, OCTAL);
	fill_zeros(header->uid, len, UIDLEN);
	len = my_itoa(header->gid, stats.st_gid, OCTAL);
	fill_zeros(header->gid, len, GIDLEN);
}

/*
 *
 
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

/*
 * 
 
	- Writes size in bytes to header->size.

																								*/

void add_size(header_t *header, struct stat stats)
{
	if (stats.st_mode != S_IFLNK)
	{
		int len = my_itoa(header->size, stats.st_size, OCTAL);
		fill_zeros(header->size, len, SIZELEN);
	}
	else
		header->size[0] = '0';
}

/*
 *

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

/*
 *

	- read mode, gid, and uid from stat struct and sum them together.
	- write mode in octal to header->mode

																									*/

void add_mode(header_t *header, struct stat stats)
{
	int mode = 0;

//	int *tar_modes = create_modes(tar_mode);
	int *stat_modes = create_modes(stat_mode);

		for (int i = 0; i < MODES_ARR_LEN; ++i)
		if (stats.st_mode & stat_modes[i])

			mode += stat_modes[i];

	int len = my_itoa(header->mode, mode, OCTAL);
	fill_zeros(header->mode, len, MODELEN);

//	free(tar_modes);
	free(stat_modes);
}

/*
 *
 
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

	for (int i = 0; i < TVERSLEN; ++i)
		header->version[i] = ' ';

	header->version[TVERSLEN - 1] = '\0';
}

void init_optional_fields(header_t *header)
{
	header->devmajor[0] = '\0';
	header->devminor[0] = '\0';
	header->linkname[0] = ' ';
	header->linkname[1] = '\0';
	header->prefix[0] = '\0';
}

/*
 * =====================================================================================
 *
 *   CREATE HEADER																																										     
 *   																									                     													 
 *    - Using the path passed in as first argument, create_header creates a tar struct following the	   
 *      basic tar convention: https://www.gnu.org/software/tar/manual/html_node/Standard.html		     
 *    - This header struct is based on the Tar Header Block, from POSIX 1003.1-1990.							       
 *    - This header struct adds a trailing null to every field											                    
 *    
 * =====================================================================================
 */

header_t *create_header(char *path, struct stat stats)
{
	header_t *header;
    header = (header_t *)calloc(1, sizeof(header_t)+1);
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
	return header;
}
