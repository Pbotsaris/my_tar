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

<<<<<<< HEAD
bool_t symlink_exists(const char *path)
{
	struct stat stats;
	int result;
	result = lstat(path, &stats);
	return (result == 0);
}

void checksum(header_t *header)
{
	unsigned int check = 0;

	check += checksum_calculator(header->name, sizeof(header->name));
	check += checksum_calculator(header->mode, sizeof(header->mode));
	check += checksum_calculator(header->uid, sizeof(header->uid));
	check += checksum_calculator(header->gid, sizeof(header->gid));
	check += checksum_calculator(header->size, sizeof(header->size));
	check += checksum_calculator(header->mtime, sizeof(header->mtime));
=======
void checksum(header_t *header)
{
	unsigned int check = 0;
	check += checksum_calculator(header->name, 100);
	check += checksum_calculator(header->mode, 10);
	check += checksum_calculator(header->uid, 8);
	check += checksum_calculator(header->gid, 8);
	check += checksum_calculator(header->size, 12);
	check += checksum_calculator(header->mtime, 12);
>>>>>>> 25b42f97fcb8a59b4321b9065a660053b73dbeb9
	check += header->typeflag;
	check += 8 * 32;
	check += checksum_calculator(header->linkname, sizeof(header->linkname));
	check += checksum_calculator(header->version, sizeof(header->version));
	check += checksum_calculator(header->uname, sizeof(header->uname));
	check += checksum_calculator(header->gname, sizeof(header->gname));
	check += checksum_calculator(header->prefix, sizeof(header->prefix));
	sprintf(header->chksum, "%0*o ", 8, check);
}

void file_info(header_t *header, struct stat stats)
{

	my_itoa(header->uid, stats.st_uid, DECIMAL);
	my_itoa(header->gid, stats.st_gid, DECIMAL);
	if (stats.st_mode != S_IFLNK)
		my_itoa(header->size, stats.st_size, DECIMAL);
	else
		header->size[0] = '0';
	// OSX
	//	my_itoa(header->mtime, stats.st_mtimespec.tv_sec, DECIMAL);
	// LINUX
	my_itoa(header->mtime, stats.st_mtim.tv_sec, DECIMAL);

	/* [> Modified time in seconds <] */
	my_itoa(header->mtime, stats.st_mtim.tv_sec, DECIMAL);

	/* [> Check Sum <] */
	checksum(header);
	//	checksum(header);
}

void add_dev_major_minor(header_t *header, struct stat stats)
{
	my_itoa(header->devmajor, (int)major(stats.st_rdev), DECIMAL);
	my_itoa(header->devminor, (int)minor(stats.st_rdev), DECIMAL);
}

void add_filetype(header_t *header, struct stat stats, char *path)
{

	if (S_ISDIR(stats.st_mode))
		header->typeflag = DIRTYPE;
	else if (S_ISREG(stats.st_mode))
	{
		if (symlink_exists(path) == TRUE)
			header->typeflag = SYMTYPE;
		else
			header->typeflag = REGTYPE;
	}
	else if (S_ISCHR(stats.st_mode))
	{
		header->typeflag = CHRTYPE;

		void add_link_or_regtype(header_t * header, char *path)
		{
			struct stat lstats;
			if (lstat(path, &lstats) == 0)
			{
				header->typeflag = SYMTYPE;
				size_t buff_size = (lstats.st_size / sizeof(char)) + 1;
				int num_bytes;
				num_bytes = readlink(path, header->linkname, buff_size);
				header->linkname[buff_size + 1] = '\0';
			}
			else
				header->typeflag = REGTYPE;
		}

		void add_filetype(header_t * header, struct stat stats, char *path)
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

		void add_uname_gname(header_t * header, struct stat stats)
		{
			struct passwd *pws;
			struct group *grp;
			pws = getpwuid(stats.st_uid);
			grp = getgrgid(stats.st_gid);

			strcpy(header->gname, grp->gr_name);
			strcpy(header->uname, pws->pw_name);
		}

		void add_mode(header_t * header, struct stat stats)
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

		void add_name(header_t * header, char *path)
		{
			size_t path_len = strlen(path);
			if (path_len < MAX_NAME_SIZE)
			{

				strcpy(header->name, path);
				header->prefix[0] = '\0';
			}

			else if (path_len < MAX_NAME_SIZE * 2)
			{
				int split_pos = path_len - MAX_NAME_SIZE + 1;
				printf("split pos %i\n", split_pos);
				else if (path_len < MAX_NAME_SIZE * 2)
				{
					int split_pos = path_len - MAX_NAME_SIZE + 1;
					strncpy(header->prefix, path, split_pos);
					header->prefix[split_pos] = '\0';
					strncpy(header->name, &path[split_pos], MAX_NAME_SIZE);
					header->name[MAX_NAME_SIZE - 1] = '\0';
				}
				else printf("%s", EXC_NAME_SIZE);
			}

			int check_byte(int block)
			{
				if (block == 512)
					return 0;
				else
					return (512 - (block % 512));
			}

			void tar(char *path, FILE *dest)
			{

				int fd = open(path, O_APPEND),
					byte_block;

				header_t header;
				struct stat stats;

				if (fd)
				{

					if (stat(path, &stats) == 0)
					{
						add_name(&header, path);
						add_mode(&header, stats);
						add_filetype(&header, stats, path);
						add_uname_gname(&header, stats);
						file_info(&header, stats);
					}
					int buff_size = (int)stats.st_size;
					char *buffer = malloc(sizeof(char) * buff_size);

					read(fd, buffer, sizeof(char) * buff_size);

					fwrite(&header, sizeof(header), 1, dest);
					fwrite(buffer, atoi(header.size), 1, dest);

					byte_block = check_byte(my_atoi(header.size));
					if (byte_block != 0)
					{
						char *zip = malloc(sizeof(char) * byte_block);
						fwrite(zip, byte_block, 1, dest);
						free(zip);
					}
					free(buffer);
					close(fd);
				}
				else
				{
					printf("ERROR\n");
					exit(1);
				}

				printf("File: %s\n", header.name);
			}

			void archive(char *path, char **argv, int argc)
			{
				struct stat stats;
				FILE *dest = fopen(path, "wb");
				int fd,
					index = 2;

				if (dest == NULL)
				{
					printf("ERROR\n");
					exit(1);
				}

				while (index < argc)
				{
					fd = open(argv[index], O_APPEND);
					lseek(fd, 0, SEEK_END);
					tar(argv[index], dest);
					index++;
				}
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
