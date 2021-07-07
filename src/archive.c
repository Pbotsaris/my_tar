#include "../include/my_tar.h"
#include "../include/messages.h"
#include "../include/header.h"
#include <stdio.h>

/*
 *
 - PRIVATE: append a data in a given path to .tar file

*/
int tar(char *path, FILE *dest)
{
	int fd = open(path, O_APPEND),
			remain_fill_block;

    char fill_header[HEADERBYTE]; // HEADERBYTE = 12
    my_memset(fill_header , '\0', HEADERBYTE);
	
    header_t *header;
    struct stat stats;
    if (fd)
	{
		if ( stat(path, &stats)  == 0)
			header = create_header(path, stats);
    	
        fwrite(header, sizeof(header_t), 1, dest);
        fwrite(fill_header, HEADERBYTE, 1, dest);

		/*  SKIP SYMLINK  */ 
		if( header->typeflag != DIRTYPE && header->typeflag != SYMTYPE){
    		long long buff_size = stats.st_size;
    		char *buffer = (char*)malloc(sizeof(char) * (buff_size + 1));
    		read(fd, buffer, buff_size);
    		buffer[buff_size-1] = '\n';
        	fwrite(buffer, buff_size, 1, dest);

		    remain_fill_block = (BLOCKSIZE - (buff_size % BLOCKSIZE));

		    if(remain_fill_block != 0){
		    	char* fill_block = malloc(sizeof(char)*remain_fill_block);
		    	my_memset(fill_block, '\0', remain_fill_block);
		    	fwrite(fill_block, remain_fill_block, 1, dest);
		    	free(fill_block);
		    }

		    free(buffer);
        }
        free(header);
		close(fd);

        return 0;

	}	else {
		printf("Error while writting to archive\n");
        return 1;
	}
}

/*
 *
 - PRIVATE: Validates if either a path is a directory

*/

bool_t is_dir(char *path)
{
	struct stat stats;
	stat(path, &stats);
	return S_ISDIR(stats.st_mode);
}

/*
 *
 - PRIVATE: Concats path as follows -  dir ++ / ++ file

*/

char *join_path(char *dir, char *file)
{
	size_t len = strlen(dir)  + strlen(file);;

	char *buffer = NULL;
	buffer = (char*)malloc( (len + 2) * sizeof(char));
	memset(buffer, '\0', len + 2);
	strcat(buffer, dir);
	strcat(buffer, "/");
	strcat(buffer, file);

	return buffer;		 

}

/*
 *
 - PRIVATE: call tar to all files in a directory but other directories

*/

void handle_dir(char *path, FILE *dest)
{
	DIR * dirp;
	struct dirent * entry;
	dirp = opendir(path); 
    
    tar(path, dest);
	if(dirp){
		while ((entry = readdir(dirp)) != NULL) {
			if (entry->d_type != DT_DIR) { 
				char *full_path =join_path(path, entry->d_name);
				tar(full_path, dest);
				free(full_path);
			}
		}
		closedir(dirp);
	}
	else {
		printf("Failed openning the directory %s\n", path);

	}
}

/*
 * =====================================================================================
 *
 *  ARCHIVE																																										     
 *   																									                     													 
 *    -  Takes an array of **paths and archieves to a tar file
 *    -  path[0] is the path to the tar file. 
 *    - Subsequent items are paths to files to be archived
 *    - paths can be both files of directories
 *
 *    
 * =====================================================================================
 */
FILE *check_file(FILE *dest, option_t option){

    if(option == c)
        return dest;

    if(option == t){
       dest = fopen(file, "wb");

       if(dest == NULL)
           return NULL;
   }
}   


int archive(char **paths, size_t paths_len, option_t option)
{

	struct stat stats;
    FILE *dest = fopen(paths[0], "wb"); 
    dest = check_file(dest, option);
    if(dest == NULL){
        printf("Couldn't open your tar\n");
        return 1;
    }
	size_t index = 1;
    
	printf("Files being archived to %s\n", paths[0]);

	//		fd = open(paths[index], O_APPEND);
	//					lseek(fd, 0, SEEK_SET);

	while (index < paths_len)
	{
		if ( stat(paths[index], &stats) == 0){
			{
				/* DIR */ 
				if(is_dir(paths[index]))
					handle_dir(paths[index], dest);
				/* FILE */ 
				else
					tar(paths[index], dest);

				index++;
			}

		}
		else {
			printf("%s\n", FILE_NOT_FOUND_ERR);
			return -1;
		}
	}

	//		close(fd);
	fclose(dest);
	return 0;
}
