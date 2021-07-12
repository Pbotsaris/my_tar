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


void make_directory(char *path)
{
    struct stat stats;   
    if (stat (path, &stats) == 0)
        printf("The directory %s already exists. Unable to extract. \n" , path);
    else
        mkdir(path, S_IWUSR | S_IXUSR | S_IRUSR | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH); 
}


int touch(int tar, header_t *header)
{
    if(header->typeflag == REGTYPE)
    {
        int dest = open(header->name, O_CREAT | O_WRONLY);
        if (dest < 0){
            printf("Could not create file %s for extraction.\n", header->name);
            return -1;
        }
        write_file(dest, tar);
        chmod(header->name, atoi(header->mode));
    }

    printf("The file %s was succesfully extracted. \n", header->name);
    return 0;
}


void extract(int tar, int file_position, int end_file)
{
    while (file_position <= end_file)
    {
        header_t *header = malloc(sizeof(header_t));
        header = get_header(tar);
        lseek(tar, ENDBLK, SEEK_CUR);

        if (header->typeflag != DIRTYPE){
            touch(tar, header);
            file_position = lseek(tar, next_header_position(header) - BLOCKSIZE, SEEK_CUR);
        }
        else
        {
            make_directory(header->name);
            file_position += BLOCKSIZE;
        }
        free(header);
    }

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
        extract(tar, file_position, end_file);

    close(tar);

    return 0;
}
