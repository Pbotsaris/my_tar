#include "../include/my_tar.h"
#include "../include/messages.h"
#include "../include/header.h"

<<<<<<< HEAD:src/archive.c
char *get_path_dir(char* path){

    size_t size = strlen(path) + 1,
        index = size;

    char *str = malloc(sizeof(char*)*strlen(path));

    while(path[index] != '/')
        index--;
  
    return strncpy(str, path, (size - index));
}

char *get_file_name(char *path){
    size_t size = strlen(path) + 1,
           index = size - 1,
           current = 0;

    char *str = malloc(sizeof(char*)*size);

    while(path[index] != '/')
        index--;
    index++;
    while(index < size){
        str[current] = path[index];
        index++;
        current++;
    }
    printf("%s\n", str);
    return str;
}

int search_match(header_t *path_header, int tar)
=======

void move_file_position(header_t *tar_header, int dest, int *current_file_location)

>>>>>>> 264613e56a0d1ed1bc9d1b6ba03d4d9e547228e0:src/archive_and_append.c
{
 if (tar_header->typeflag == REGTYPE || tar_header->typeflag == LNKTYPE)
            *current_file_location = lseek(dest, next_header_position(tar_header), SEEK_CUR);
        else
            *current_file_location += BLOCKSIZE;
}

/*
 *
 - PRIVATE: Searches for maching files in archive for when appending (-uf)

*/

int search_match(header_t *path_header, int dest)
{
    int size = lseek(dest, 0, SEEK_END),
        current_file_location = lseek(dest, 0, SEEK_SET);

    while (current_file_location <= size)
    {
        header_t *tar_header;
        if (!(tar_header = malloc(sizeof(BLOCKSIZE))))
            return -1;

        tar_header = get_header(dest);
        lseek(dest, ENDBLK, SEEK_CUR);

        if (strcmp(tar_header->name, path_header->name) == 0){

            if (strcmp(tar_header->mtime, path_header->mtime) != 0){
                /* found match */
                free(tar_header);
                return 0;
            }
            else {
                printf("Cannot append. This version of %s is older than in the archive.\n", tar_header->name);
                free(tar_header);
                return -1;
            }
        }
        else {
            move_file_position(tar_header, dest, &current_file_location);
            free(tar_header);
        }
    }

    /* not found */
     printf("%s: Could not find a matching file in the archive\n", path_header->name);
    return -1;

}

/*
 *
 - PRIVATE: append a data in a given path to .tar file

*/

int tar(char *path, int dest, option_t option)
{

    header_t *header;
    struct stat stats;
<<<<<<< HEAD:src/archive.c
    
    if (fd)
    {
        if(lstat(path, &stats) == 0){
        }
            stat(path, &stats);
    
        header = create_header(path, stats);
=======

    /* stat check done in parent */
    stat(path, &stats);
    header = create_header(path, stats);

    char fill_header[HEADERBYTE]; /* HEADERBYTE = 12 */
    my_memset(fill_header, '\0', HEADERBYTE);

    if(option == r)
        lseek(dest, 0, SEEK_END);
    else if (option == u && (search_match(header, dest) != 0))
    {
        free(header);
        return -1;
    }
>>>>>>> 264613e56a0d1ed1bc9d1b6ba03d4d9e547228e0:src/archive_and_append.c

    write(dest, header, sizeof(header_t));
    write(dest, fill_header, HEADERBYTE);

    /*  WRITE CONTENT ONLY ON REG AND HARD LINK  */
    if (header->typeflag == LNKTYPE || header->typeflag == REGTYPE)
    {
        int fd = open(path, O_APPEND),
            remain_fill_block;

        if (fd)
        {
            long long buff_size = stats.st_size;
            char *buffer = (char *)malloc(sizeof(char) * (buff_size + 1));
            read(fd, buffer, buff_size);
            buffer[buff_size] = '\n';
            write(dest, buffer, buff_size);

            remain_fill_block = (BLOCKSIZE - (buff_size % BLOCKSIZE));

            if (remain_fill_block != 0)
            {
                char *fill_block = malloc(sizeof(char) * remain_fill_block);
                my_memset(fill_block, '\0', remain_fill_block);
                write(dest, fill_block, remain_fill_block);
                free(fill_block);
            }
            close(fd);
        }
        else
        {
            printf("Error while writting to archive\n");
            free(header);
            return -1;
        }
    }

    free(header);
    return 0;
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
 - PRIVATE: Validates if either a path is first in first out 

*/


char *join_path(char *dir, char *file)
{
    size_t len = strlen(dir) + strlen(file);

    char *buffer = NULL;
    buffer = (char *)malloc((len + 2) * sizeof(char));
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

int handle_dir(char *path, int dest, option_t option)
{
    DIR *dirp;
    struct dirent *entry;
    dirp = opendir(path);

    tar(path, dest, option);
    if (dirp)
    {
        while ((entry = readdir(dirp)) != NULL)
        {
            if (entry->d_type != DT_DIR)
            {
                char *full_path = join_path(path, entry->d_name);
                tar(full_path, dest, option);
                free(full_path);
            }
        }
        closedir(dirp);
        return 0;
    }
    else
    {
        printf("Failed openning the directory %s\n", path);
        return -1;
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
<<<<<<< HEAD:src/archive.c
int check_file_type(char *path, int dest, option_t option){

    struct stat stats;
    /* path = "test_files/linkfile.txt"; */
    if (stat(path, &stats) == 0)
    {
        if (is_dir(path))
            handle_dir(path, dest, option);
        else
            tar(path, dest, option);
    }
    else
    {
        printf("The path provided is incorrect\n");
        return 1;
    }
    return 0;
}


int archive(char **path, size_t paths_len, option_t option)
{
=======

int archive(char **path, size_t paths_len, option_t option)
{
    struct stat stats;
    int tar_result = 0;
>>>>>>> 264613e56a0d1ed1bc9d1b6ba03d4d9e547228e0:src/archive_and_append.c

    int dest = open(path[0], O_CREAT | O_RDWR);
    chmod(path[0], S_IWUSR | S_IXUSR | S_IRUSR | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);

    if (dest < 0)
        return 1;

    size_t index = 1;
<<<<<<< HEAD:src/archive.c
    struct stat stats;
    while (index < paths_len)
    {
        if((check_file_type(path[index], dest, option) == 0))
            index++;
        else return 1;
    }

    printf("Archiving was successful!\n");
=======
    while (index < paths_len)
    {
        if (stat(path[index], &stats) == 0)
        {
            if (is_dir(path[index]))
                tar_result = handle_dir(path[index], dest, option);
            else
                tar_result = tar(path[index], dest, option);
            index++;
        }
        else
        {
            printf("The path %s was incorrect\n", path[index]);
            return -1;
        }
    }

    if(tar_result > 0)
        printf("Archiving was successful!\n");

>>>>>>> 264613e56a0d1ed1bc9d1b6ba03d4d9e547228e0:src/archive_and_append.c
    close(dest);
    return 0;
}
