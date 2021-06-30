
/*
 * =====================================================================================
 *
 *       Filename:  my_tar.c
 *
 *    Description:  A naive implementation with the infamous tar
 *
 *        Version:  0.1
 *        Created:  06/22/2021 16:05:36
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Khalil Masri, Pedro Botsaris
 *   Organization:  ELU
 *
 * =====================================================================================
 */

#include "my_tar.h"

int main(int argc, char *argv[])
{

    // check_option(argv);

    if (argc < 2)
        return 0;

    header_t *header;

    for (int i = 0; i < argc; ++i)
    {
        if (i == 0)
            continue;
        if (argv[i][0] == '-')
            continue;

         header = create_header(argv[i]);
    }

//    archive(argv[1], argv, argc);

     printf("name: %s\nmode: %s\n", header->name, header->mode);
    printf("size: %s\n", header->size);
    printf("uid:%s\ngid: %s\n", header->uid, header->gid);
    printf("gname: %s\nuname: %s\n", header->gname, header->uname);
    printf("time modified: %s\nchecksum: %s\ntypeflag: %c\n", header->mtime, header->chksum, header->typeflag);
    printf("linkname: %s\nMagic: %s\nversion: %s\n", header->linkname, header->magic, header->version);
    printf("devmajor: %s, devminor %s\n", header->devmajor, header->devminor);
    printf("Prefix: %s\n", header->prefix);

    free(header);

    return 0;
}
