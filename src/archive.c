#include "my_tar.h"

void tar(char *path, FILE *dest)
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
		// don't writeif symlink
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
		free(header);
		close(fd);

	}	else {
		printf("Error while writting to archive\n");
	}
}

 bool_t is_dir(char *path)
{
		struct stat stats;
	    stat(path, &stats);
 		   return S_ISDIR(stats.st_mode);
}


int count_dir(char **paths, size_t paths_len)
{
	int count;
	for (size_t i = 0; i < paths_len; ++i) {
		if(is_dir(paths[i]))
			count++;
	}	

	return count;	

}

void archive(char **paths, size_t paths_len)
{

	struct stat stats;
	FILE *dest = fopen(paths[0], "wb");
	int fd; 
	size_t index = 1;
	if (dest == NULL){
		printf("Failed to open archive file\n");
			return;
	}

			printf("Files being archived to %s\n", paths[0]);

	while (index < paths_len)
	{
		if ( stat(paths[index], &stats)  == 0){
			{
				fd = open(paths[index], O_APPEND);
				lseek(fd, 0, SEEK_SET);
				// skip dir
				if(!is_dir(paths[index]))
					tar(paths[index], dest);
				index++;
				close(fd);
			}

			fclose(dest);
		}
		else {
			printf("File not found.\n");
			return;
		}
	}
	return;
}
