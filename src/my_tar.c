
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

    /*
      Handling options
                                                            */
    char **paths;
    option_t options;
    bool_t is_skip_options = search_flag(argv, 's');
    if(is_skip_options == FALSE){
        // check for options
        options = check_option(argv);
        if(options == ERROROPT || options == MISSING_F || options == NONE)
            return 0;
    }
    else
        printf("skipping options for debug.\n");

    /*
      prepare paths for ingest
                                                            */
    int path_start_index = find_paths_start_index(argv);
    paths = argv + path_start_index;
    size_t paths_len = argc - path_start_index;
    bool_t is_tar_valid = validate_tar_extention(paths[0]);

    /* wrong extention */
    if(is_tar_valid == FALSE){
        printf("Archives must have a .tar extention.\n");
        return 0;
    }

    /* no path to archive */
    if(paths_len == 1){
        printf("You must provide a .tar file and a path to file to archive.\n");
        return 0;
    }

    /*
       Handle files and dirs
                                                            */
    size_t num_dirs = count_dirs(paths, paths_len);
    size_t dir_indexes[num_dirs > 0 ? num_dirs : 1];
    int num_dir_files = 0;

    if(num_dirs > 0) {
    get_dir_indexes(paths, dir_indexes, paths_len);
    num_dir_files = count_dir_files(paths, dir_indexes, num_dirs);
    }
    else
        dir_indexes[0] = -1;


    header_t *headers[paths_len + num_dir_files];
    int num_headers = archive(paths, paths_len, dir_indexes, headers);


    /* 
       -  Debug mode
                                                            */
    bool_t is_debug = search_flag(argv, 'd');
    if(is_debug == TRUE)
        debug_header(headers[0]);

    // search for 'l' for listing TODO: WE DONT NEED THIS FOR L. THIS IS BEING HANDLED IN SELECT OPTION
    bool_t is_ls = search_flag(argv, 'l');
    if(is_ls == TRUE){
        my_ls_tar(argv[3]);
    }

    for(int i = 0; i < num_headers; i++) 
        free(headers[i]);

    return 0;
}
