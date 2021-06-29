#include "my_tar.h"

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

		create_header(path);
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
