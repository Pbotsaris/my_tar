#include "my_tar.h"

int check_byte(int block)
{
    if (block == 512)
        return 0;
    else
        return (512 - (block % 512));
}

int my_ls_tar(char *path)
{
    int tar = open(path, O_RDWR);

    if (!tar)
        return 1;

    header_t header;
    lseek(tar, 0, SEEK_SET);
    int current = 0;

    struct stat statbuf;

    if (stat(path, &statbuf) == -1)
    {
        return 1;
    }
    printf("%ld\n", statbuf.st_size);
    //     // while (current <= statbuf.st_size)
    //     // {

    //     //     read(tar, &header, sizeof(header));
    //     //     if (current < 0)
    //     //         return 1;
    //     //     printf("FILE NAME TAR: %s\n", header.name);
    //     //     current += atoi(header.size);
    //     //     printf("%d header size \n", current);
    //     //     lseek(tar, current, SEEK_CUR); // header.name[0] = '\0';
    // }
    close(tar);
}