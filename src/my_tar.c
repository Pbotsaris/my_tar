
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

    header_t *header;

    for (int i = 0; i < argc; ++i)
    {
        if (i == 0)
            continue;
        if (argv[i][0] == '-')
            continue;

        header = create_header(argv[i]);
    }
    
    printf("group name: %s\n user name: %s\n", header->gname, header->uname);
    printf("name: %s\n prefix: %s\n", header->name, header->prefix);
    printf("mode in char(octal): %s\nUSER ID: %s\nGROUP OWNER ID: %s\nSize: %s\nLink: %s\n", header->mode, header->uid, header->gid, header->size, header->linkname);

    free(header);

    return 0;
}
