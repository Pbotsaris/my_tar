#include "../include/my_tar.h"
#include "../include/messages.h"
#include "../include/header.h"

/*
 *
 - PRIVATE: Write data from .tar to file created on extraction

*/

void write_file(int dest, int tar)
{
    char *buffer[BLOCKSIZE];
    read(tar, buffer, BLOCKSIZE);
    write(dest, buffer, BLOCKSIZE);
    close(dest);
}

/*
 *
 - PRIVATE: Creates a directory
            -> returns 0 =  Success
            -> returns -1 = failure 
*/

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

/*
 *
 - PRIVATE: Creates a file and sets the correct permission based on .tar header
            -> returns 0 =  Success
            -> returns -1 = failure 

*/

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

    if(header->name[0] != '\0')
        printf("The file %s was succesfully extracted. \n", header->name);

    return 0;
}


/*
 *
 - PRIVATE: Check weather a directory exists in a given path
            -> returns 1 = directory found
            -> returns 0 || 1 = directory not found

*/

int dir_exists(char *path)
{
    DIR* dir = opendir(path);
    if (dir) {
        return 1;
        closedir(dir);
    } else if (ENOENT == errno) 
        /* doesn't exist */
        return 0;
    else 
        /* another reason */
        return -1;
}

/*
 *
 - PRIVATE: Counts the number of directories and sub directories in a path
            e.g. dir/sub-dir/sub-dir/file  =  3
            -> returns the count

*/

int count_dirs_in_path(char *path, int len)
{
    int count = 0;
    for (int i = 0; i < len; ++i) 
        if(path[i] == '/') 
            count++;

    return count;
}

/*
 *
 - PRIVATE: Checks wheater a path contains directory or just a file
                e.g. file.txt = true,  dir/file.txt = false 
            -> returns the leftmost position of the directory name in relation to the path string.
                e.g. dir/sub-dir/file = [3, 11]
            -> returns an array [-1 ] if path does not contain directories 

*/

int *has_path_dir(char *path)
{
    size_t len = strlen(path);
    int num_dirs = count_dirs_in_path(path, len);
    int pos_index = 0;

    int *positions;
    positions = (int*)malloc(num_dirs * sizeof(int));

    for (size_t i = 0; i < len; ++i) 
        if(path[i] == '/') 
        {
            positions[pos_index] = i;
            pos_index++;
        }
    if(pos_index == 0)
        positions[pos_index] = -1;

    return positions;
}

/*
 *
 - PRIVATE: Extract a single directory path based base on its leftmost position in the path string
                e.g. path = dir/sub-dir/file.txt
                        pos = 3 then returns dir
                        pos = 11 then returns dir/sub-dir
*/

char *extract_dirname_from_path(char *path, int pos)
{
    char *dirname = (char*)malloc((pos + 1) * sizeof(char));
    int dirname_index = 0;
    for (int i = 0; i < pos; i++)
    {
        dirname[dirname_index] = path[i];
        dirname_index++;
    }
    dirname[pos] = '\0';
    return dirname;
}

/*
 * =====================================================================================
 *
 *  EXTRACT - PRIVATE																																										     
 *   																									                     													 
 *    -  Takes a tar file, the initial position of the lseek cursor and the total length of the file
 *    -  Scans through tar file extracting the archived files and directories
 *    -  Sets te permission accordingly.
 *    -  Recreates the directory structure of the file if desn't exiss.
 *
 *    
 * =====================================================================================
 */

int extract(int tar, int file_position, int end_file)
{
    while (file_position <= end_file)
    {
        header_t *header = malloc(sizeof(header_t));
        header = get_header(tar);
        lseek(tar, ENDBLK, SEEK_CUR);

        if (header->typeflag != DIRTYPE){
            /* check for path has directory and return position of dirname in path string */
            int *pos = has_path_dir(header->name);
            if (pos[0] >= 0) 
            {
             int pos_len = count_dirs_in_path(header->name, strlen(header->name));
             for(int i = 0;  i < pos_len; i++)
             {
                char *dirname =  extract_dirname_from_path(header->name, pos[i]);
            /* Creates the  directory if it doesn't exists */
                if (dir_exists(dirname) < 1)
                     make_directory(dirname);
                free(dirname);
             }
             free(pos);
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

/*
 * =====================================================================================
 *
 *  LIST - PRIVATE																																										     
 *   																									                     													 
 *    -  Takes a tar file, the initial position of the lseek cursor and the total length of the file
 *    -  Lists the pathnames contained in header->name for every archived file or directory
 *    -> returns 0 = Success
 *    -> returns -1 = Failure 
 *
 *    
 * =====================================================================================
 */

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

/*
 * =====================================================================================
 *
 *   LIST OR EXTRACT
 *   																									                     													 
 *    -  Combines both list and extract in one public interfacing function
 *    -> returns 0 = Success
 *    -> returns -1 = Failure 
 *
 *    
 * =====================================================================================
 */

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
