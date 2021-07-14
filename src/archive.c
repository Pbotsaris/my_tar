#include "../include/my_tar.h"
#include "../include/messages.h"
#include "../include/header.h"

/*
 *
 - PRIVATE: Searches for maching files in archive for when appending (-uf)

*/

int search_match(header_t *path_header, int tar)
{
    int size = lseek(tar, 0, SEEK_END),
        current_file_location = lseek(tar, 0, SEEK_SET);

    while (current_file_location <= size)
    {
        header_t *tar_header;
        if (!(tar_header = malloc(sizeof(BLOCKSIZE))))
            return 1;

        tar_header = get_header(tar);
        lseek(tar, ENDBLK, SEEK_CUR);

        if (strcmp(tar_header->name, path_header->name) == 0 &&
                strcmp(tar_header->mtime, path_header->mtime) == 0){
            free(tar_header);
            return 1;
        }
        
        if (tar_header->typeflag != DIRTYPE)
            current_file_location = lseek(tar, next_header_position(tar_header), SEEK_CUR);
        else
            current_file_location += BLOCKSIZE;
        
        free(tar_header);
    }
    return 0;
}

/*
 *
 - PRIVATE: append a data in a given path to .tar file

*/

int tar(char *path, int dest, option_t option)
{

    header_t *header;
    struct stat stats;

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

    write(dest, header, sizeof(header_t));
    write(dest, fill_header, HEADERBYTE);

    /*  WRITE CONTENT ONLY ON REG AND HARD LINK  */
    if (header->typeflag == LNKTYPE && header->typeflag == REGTYPE)
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
    ;

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
        return 1;
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

int archive(char **path, size_t paths_len, option_t option)
{
    struct stat stats;

    int dest = open(path[0], O_CREAT | O_RDWR);
    chmod(path[0], S_IWUSR | S_IXUSR | S_IRUSR | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);

    if (dest < 0)
        return 1;

    size_t index = 1;
    while (index < paths_len)
    {
        if (stat(path[index], &stats) == 0)
        {
            if (is_dir(path[index]))
                handle_dir(path[index], dest, option);
            else
                tar(path[index], dest, option);
            index++;
        }
        else
        {
            printf("The path %s was incorrect\n", path[index]);
            return 1;
        }
    }
    printf("Archiving was successful!\n");
    close(dest);
    return 0;
}
