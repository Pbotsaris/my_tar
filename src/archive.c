#include "my_tar.h"

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
			header = *create_header(path);
		}
		int buff_size = (int)stats.st_size;
		char *buffer = malloc(sizeof(char) * buff_size + 1);

		read(fd, buffer, buff_size);
		buffer[buff_size] = '\0';

		printf("size %d\n", buff_size);

		fwrite(&header, sizeof(header), 1, dest);

		fwrite(buffer, buff_size, 1, dest);

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
		lseek(fd, 0, SEEK_SET);
		tar(argv[index], dest);
		index++;
		close(fd);
	}

	fclose(dest);
}