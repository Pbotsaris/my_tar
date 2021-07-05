#include "my_tar.h"

header_t *tar(char *path, FILE *dest)
{
	int fd = open(path, O_APPEND),
			remain_fill_block;

	header_t *header;
	struct stat stats;

	if (fd)
	{
		if ( stat(path, &stats)  == 0)
		header = create_header(path, stats);

		long long buff_size = stats.st_size;
		char *buffer = (char*)malloc(sizeof(char) * buff_size + 1);
		read(fd, buffer, buff_size);
		buffer[buff_size-1] = '\0';

		fwrite(header, BLOCKSIZE, 1, dest);
		// don't write if link
		if(header->typeflag != SYMTYPE)
				fwrite(buffer, buff_size, 1, dest);

		remain_fill_block = (BLOCKSIZE - (buff_size % BLOCKSIZE));

		if(remain_fill_block != 0){
			char* fill_block = malloc(sizeof(char)*remain_fill_block);
			my_memset(fill_block, '\0', remain_fill_block);
			fwrite(fill_block, remain_fill_block, 1, dest);
			free(fill_block);
		}

		free(buffer);
		close(fd);
		printf("File name in archive: %s\n", header->name);
		return header;

	}	else {

	
		printf("ERROR\n");
		//	exit(1);
		return NULL;
	}
}

int archive(char **paths, size_t paths_len, header_t *headers[])
{
	struct stat stats;
	FILE *dest = fopen(paths[0], "wb");
	int fd; 
	size_t index = 1;
	if (dest == NULL)
	{
		printf("Error creating archive file\n");
		return -1;
	}

	while (index < paths_len)
	{
		fd = open(paths[index], O_APPEND);
		lseek(fd, 0, SEEK_SET);
		// header per file
		headers[index - 1] = tar(paths[index], dest);
		index++;
		close(fd);
	}

	fclose(dest);

	return index - 1;

}
