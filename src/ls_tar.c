#include "my_tar.h"

int my_ls_tar(char *path)
{
    int tar = open(path, O_RDWR),
        counter = 1,
        block_size_octal = decimal_to_octal(BLOCKSIZE),
        current_file_position = 0,
        end_file;

    char buffer[BLOCKSIZE];

    float size;

    if (tar < 0)
        return 1;

    end_file = lseek(tar, 0, SEEK_END);
    lseek(tar, 0, SEEK_SET);

    while (current_file_position <= end_file)
    {
        read(tar, buffer, NAMELEN); // READ THE NAME

        if (buffer[0] != '\0') // CHECKS IF BUFFER ISN'T EMPTY TO PRINT HEADER NAME
            printf("%s\n", buffer);

        lseek(tar, JMPSIZE, SEEK_CUR); // LOOKS FOR THE SIZE OF THE FILE
        read(tar, buffer, SIZELEN);    // READS THE SIZE OF THE FILE

        size = atoi(buffer);          // SIZE TO INT
        lseek(tar, ENDBLK, SEEK_CUR); // SEEKS THE END OF THE HEADER BLOCK

        if (size > block_size_octal)
        {
            size /= block_size_octal; // COUNTING HOW MANY BLOCKS ARE SAVED FOR CONTENT
            for (int i = 0; (size - i) >= 1; i++)
                counter++;
        }
        current_file_position = lseek(tar, sizeof(buffer) * (counter), SEEK_CUR); // SEEKS THE NEXT HEADER AND SAVES THAT TO SEE IF WE ARRIVED TO THE END OF THE FILE

        counter = 1;
    }

    close(tar);
}
