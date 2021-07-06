
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

    char **paths;
    int index = -1;
    option_t options;

    /*
        DEBUG  
                                                            */

    if((index = search_flag(argv, 'd')) > 0){
         debug_header(argv[index + 1]);
         printf("%s\n", argv[index]);
         return 0;
       }

    /*
         OPTIONS  
                                                            */
        options = check_option(argv);
        if(options == ERROROPT || options == MISSING_F || options == NONE)
            return 0;

    /*
        SPLIT ARGV 

                                                            */
    int path_start_index = find_paths_start_index(argv);
    paths = argv + path_start_index;
    size_t paths_len = argc - path_start_index;
    bool_t is_tar_valid = validate_tar_extention(paths[0]);
    if(is_tar_valid == FALSE){
        printf("Archives must have a .tar extention.\n");
        return 0;
    }

    /*
         ARCHIVE
                                                            */

    if(paths_len == 1 && options == c ){
        printf("You must provide a .tar file and a path to file to archive.\n");
        return 0;
    }
    if(paths_len >= 2 && options == c)
           archive(paths, paths_len);

      /*
           LIST TAR 
                                                            */

    if(options == t){
        my_ls_tar(paths[0]);
    }


    return 0;
}
