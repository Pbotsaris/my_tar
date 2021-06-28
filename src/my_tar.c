
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

    archive(argv[1], argv, argc);

    // printf("name: %s\n prefix: %s\n", header->name, header->prefix);
    // printf("type: %c\n", header->typeflag);
    // printf("dev minor: %s , dev major: %s\n", header->devminor, header->devmajor);
    // printf("mode in char(octal): %s\nUSER ID: %s\nGROUP OWNER ID: %s\nSize: %s\n", header->mode, header->uid, header->gid, header->size);
    // printf("Modified time in seconds: %s\n", header->mtime);
    // printf("Link name: %s\n", header->linkname);
    // printf("CHKSUM: %s\n", header->chksum);
    // printf("group name: %s\n user name: %s\n", header->gname, header->uname);
    // ls_tar(argv[1]);
    free(header);

    return 0;
}
