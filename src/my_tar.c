
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

    if(argv[1][0] == '-' && argv[1][1] == 'd'){ 
        printf("%s\n", argv[2]);
        header =  archive(argv[2], argv, argc);
        debug_header(header);
    }
    else if(argv[1][0] == '-') {
        printf("invalid option\n");
        return 0;
    }
    else {
        header =  archive(argv[1], argv, argc);
    }

    free(header);

    return 0;
}
