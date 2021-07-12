#include "../include/my_tar.h"
#include "../include/messages.h"
#include "../include/header.h"

int search_match(header_t *path_header, int tar)
{
    int size = lseek(tar, 0, SEEK_END) - BLOCKSIZE,
        current_file_location = 0;

    lseek(tar, 0, SEEK_SET);
    while (current_file_location <= size)
    {
        header_t *tar_header;
        if (!(tar_header = malloc(sizeof(header_t))))
            return -1;

        tar_header = get_header(tar);
        

        if (strcmp(tar_header->name, path_header->name) == 0 && 
                strcmp(tar_header->mtime, path_header->mtime) == 0)
            return 1;

        if (tar_header->typeflag != DIRTYPE)
            current_file_location = lseek(tar, next_header_position(tar_header), SEEK_CUR);
        else
            current_file_location = +BLOCKSIZE;
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
    int fd = open(path, O_APPEND),
        remain_fill_block;

    char fill_header[HEADERBYTE]; // HEADERBYTE = 12
    my_memset(fill_header, '\0', HEADERBYTE);

    header_t *header;
    struct stat stats;
    
    if(option == r)
        lseek(dest, 0, SEEK_END);

    if (fd)
    {


        if (stat(path, &stats) == 0)
            header = create_header(path, stats);

        if (option == u && (search_match(header, dest) != 0))
        {
            free(header);
            close(fd);
            return 1;
        }
        

        write(dest, header, sizeof(header_t));
        write(dest, fill_header, HEADERBYTE);

        /*  SKIP SYMLINK  */
        if (header->typeflag != DIRTYPE && header->typeflag != SYMTYPE)
        {
            long long buff_size = stats.st_size;
            char *buffer = (char *)malloc(sizeof(char) * (buff_size + 1));
            read(fd, buffer, buff_size);
            buffer[buff_size - 1] = '\n';
            write(dest, buffer, buff_size);

            remain_fill_block = (BLOCKSIZE - (buff_size % BLOCKSIZE));

            if (remain_fill_block != 0)
            {
                char *fill_block = malloc(sizeof(char) * remain_fill_block);
                my_memset(fill_block, '\0', remain_fill_block);
                write(dest, fill_block, remain_fill_block);
                free(fill_block);
            }

            free(buffer);
        }
        free(header);
        close(fd);

        return 0;
    }
    else
    {
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
    {
        printf("Could't open tar\n");
        return 1;
    }

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
            printf("The path provided is incorrect\n");
            return 1;
        }
    }
    printf("Archiving was successful!\n");
    close(dest);
    return 0;
}
