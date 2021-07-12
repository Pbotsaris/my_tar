#include "../include/my_tar.h"
#include "../include/messages.h"
#include "../include/header.h"

void write_file(int dest, int tar)
{
    char *buffer[BLOCKSIZE];
    read(tar, buffer, BLOCKSIZE);
    write(dest, buffer, BLOCKSIZE);
    close(dest);
}

int make_directory(char *path)
{
    struct stat stats;   
    if (stat (path, &stats) == 0)
    {
        printf("The directory %s already exists. Unable to extract. \n" , path);
        return -1;
    }
    else
    {
        mkdir(path, S_IWUSR | S_IXUSR | S_IRUSR | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH); 
        return 0;
    }
}

int touch(int tar, header_t *header)
{
    if(header->typeflag == REGTYPE)
    {
        int dest = open(header->name, O_CREAT | O_WRONLY);
        if (dest < 0){
            printf("failed to extract. File %s could not be created.", header->name);
            return -1;
        }
        write_file(dest, tar);
        chmod(header->name, octal_to_decimal(atoi(header->mode)));
    }

    printf("The file %s was succesfully extracted. \n", header->name);
    return 0;
}

int dir_exists(char *path)
{

DIR* dir = opendir(path);
if (dir) {
    return 1;
    closedir(dir);

} else if (ENOENT == errno) {
    /* doesn't exist */
    return 0;
} else {
    /* failed to open for another reason */
    return -1;
}    

}


int has_path_dir(char *path)
{
    size_t len = strlen(path);
    int pos = -1;

    for (size_t i = 0; i < len; ++i) 
        if(path[i] == '/') 
            pos = i;

    return pos;
}

  char *extract_dirname_from_path(char *path, int pos)
{
    char *dirname = (char*)malloc((pos + 1) * sizeof(char));
    for (int i = 0; i < pos; i++)
        dirname[i] = path[i];
    
    dirname[pos] = '\0';

    return dirname;
}


int extract(int tar, int file_position, int end_file)
{
    while (file_position <= end_file)
    {
        header_t *header = malloc(sizeof(header_t));
        header = get_header(tar);
        lseek(tar, ENDBLK, SEEK_CUR);

        if (header->typeflag != DIRTYPE){
            int pos;
            /* check for path has directory and create the directory if doesn't exist */
            if ((pos = has_path_dir(header->name)) >= 0) 
            {
                char *dirname =  extract_dirname_from_path(header->name, pos);
                if (dir_exists(dirname) < 1)
                    make_directory(dirname);

                free(dirname);
            }

            if (touch(tar, header) == 0)
                file_position = lseek(tar, next_header_position(header) - BLOCKSIZE, SEEK_CUR);
            else
                return -1;
        }
        else
        {
            if (make_directory(header->name) == 0)
                file_position += BLOCKSIZE;
            else
                return -1;
        }
        free(header);
    }

    return 0;

}

void list(int tar, int file_position, int end_file)
{
    while (file_position <= end_file)
    {
        header_t *header = malloc(sizeof(header_t));

        header = get_header(tar);
        /* resposition only on list */
        lseek(tar, ENDBLK, SEEK_CUR);

        if (header->name[0] != '\0')
            printf("%s\n", header->name);
        if (header->typeflag != DIRTYPE)
            file_position = lseek(tar, next_header_position(header), SEEK_CUR);
        else
            file_position += BLOCKSIZE;

        free(header);
    }
}

int list_or_extract(char *path, option_t options)
{

    int tar = open(path, O_RDWR),
        file_position = 0,
        end_file = (lseek(tar, 0, SEEK_END) - BLOCKSIZE);

    lseek(tar, 0, SEEK_SET);

    if(options == t)
        list(tar, file_position, end_file);

    if(options == x)
        if(extract(tar, file_position, end_file) < 0)
            return -1;

    close(tar);

    return 0;
}
