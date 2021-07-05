#include "my_tar.h"

header_t *tar(char *path, FILE *dest)
{

	int fd = open(path, O_APPEND),
		byte_block;
	header_t *header;
	struct stat stats;
	if (fd)
	{
		if (stat(path, &stats) == 0)
			header = create_header(path);

		int buff_size = (int)stats.st_size;
		char *buffer = malloc(sizeof(char) * buff_size + 1);
		read(fd, buffer, buff_size);
		buffer[buff_size] = '\0';

		fwrite(header, sizeof(header_t), 1, dest);
		fwrite(buffer, buff_size, 1, dest);

		free(buffer);
		close(fd);
		return NULL;
	}
	else
	{
		printf("ERROR\n");
	//	exit(1);
	return NULL;
	}
	printf("File name in archive: %s\n", header->name);
	return header;

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
	//	exit(1);
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
