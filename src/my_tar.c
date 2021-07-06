
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

    // search for -s  for skipping options
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

    // prepare paths for ingest
    int path_start_index = find_paths_start_index(argv);
    // offset argv to path_start_index. Store in paths pointer.
    paths = argv + path_start_index;
    // get the actual length of the paths array.
    size_t paths_len = argc - path_start_index;
    bool_t is_tar_valid = validate_tar_extention(paths[0]);

    // return error with wrong extention
    if(is_tar_valid == FALSE){
        printf("Archives must have a .tar extention.\n");
        return 0;
    }

    // didn't provide a path to archive
    if(paths_len == 1){
        printf("You must provide a .tar file and a path to file to archive.\n");
        return 0;
    }
    // archive returns the number of headers it create
    header_t *file_headers[paths_len - 1];
    int num_headers = archive_file(paths, paths_len, file_headers);

    // search fo 'd' for debug mode
    bool_t is_debug = search_flag(argv, 'd');
    if(is_debug == TRUE)
        debug_header(file_headers[0]);

    // search for 'l' for listing TODO: WE DONT NEED THIS FOR L. THIS IS BEING HANDLED IN SELECT OPTION
    bool_t is_ls = search_flag(argv, 'l');
    if(is_ls == TRUE){
        my_ls_tar(argv[3]);
    }

    for(int i = 0; i < num_headers; i++) 
        free(file_headers[i]);

    return 0;
}
