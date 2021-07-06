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
		close(fd);
		printf("File name in archive: %s\n", header->name);
		return header;

	}	else {
		printf("Error while writting to archive\n");
		return NULL;
	}
}

int handle_dir(char *path, header_t *headers[], FILE *dest, int header_index)
{
	DIR * dirp;
	struct dirent *entry;
	int index = header_index;

	// no need for error handling here
	dirp = opendir(path);
		while ((entry = readdir(dirp)) != NULL) {
			if (entry->d_type != DT_DIR) { 
			  headers[index] = tar(entry->d_name, dest);
			}
		}
	return index;

}

 bool_t is_dir(char *path)
{
		struct stat stats;
	    stat(path, &stats);
 		   return S_ISDIR(stats.st_mode);
}

int count_files(char *path)
{
	int file_count = 0;
	DIR * dirp;
	struct dirent *entry;

	if((dirp = opendir(path)) == 0) {
		while ((entry = readdir(dirp)) != NULL) {
			/* don't count nested dirs */
			if (entry->d_type != DT_DIR) { 
				file_count++;
			}
		}
	}
	else
	{
		printf("Could not read the directory %s\n", path);
		closedir(dirp);
		return -1;
	}
	closedir(dirp);
	return file_count;
}

// PUBLIC

int count_dirs(char **paths, size_t paths_len)
{
	int count = 0;
	for (size_t i = 0; i < paths_len; ++i) {
		if(is_dir(paths[i]))
			count++;
	}	
	return count;	
}


void get_dir_indexes(char **paths, size_t *dir_indexes, size_t paths_len)
{
	int j;
	for (size_t i = 0; i < paths_len; ++i) {
		if(is_dir(paths[i]))
			dir_indexes[j] = i;
		j++;
	}
}

int count_dir_files(char **paths, size_t *dir_indexes, size_t dir_indexes_len)
{
   size_t i;
	 int total_num_files;
	for (i = 0; i < dir_indexes_len; ++i) {
		total_num_files += count_files(paths[dir_indexes[i]]);
	}
	return total_num_files;
}

int archive(char **paths, size_t paths_len,size_t *dir_indexes, header_t *headers[])
{
	struct stat stats;
	FILE *dest = fopen(paths[0], "wb");
	int fd; 
	int dir_counter = 0;
	int header_index = 0;
	size_t index = 1;
	bool_t has_dir = dir_indexes < 0 ? FALSE : TRUE;

	if (dest == NULL)
	{
		printf("Error creating archive file\n");
		return -1;
	}

	while (index < paths_len)
	{
		if ( stat(paths[index], &stats)  == 0){
			{
				fd = open(paths[index], O_APPEND);
				lseek(fd, 0, SEEK_SET);
				// dir
				if(has_dir == TRUE && index == dir_indexes[dir_counter]){
				header_index = handle_dir(paths[index], headers, dest, header_index);
				} else {
				// files
				headers[header_index] = tar(paths[index], dest);
				index++;
				header_index++;
				}
				close(fd);
			}

			fclose(dest);
		}
		else {
			printf("File not found.\n");
			return -1;
		}
	}
	return index - 1;
}
