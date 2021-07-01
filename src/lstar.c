#include "my_tar.h"

int d_to_o(int decimal)
{
    int octal = 0;
    int num_digits = 1;
    int temp = decimal;
    while (temp != 0)
    {
        octal += (temp % 8) * num_digits;
        temp /= 8;
        num_digits *= 10;
    }
    return octal;
}

int my_ls_tar(char *path)
{
    int tar = open(path, O_RDWR),
        counter = 1,
        block_size_octal = d_to_o(BLOCKSIZE),
        index = 0,
        current_file_position = 0,
        file_size;

    char buffer[BLOCKSIZE];

    float size;

    if (tar < 0)
        return 1;

    struct stat stats;
    stat(path, &stats);
    file_size = stats.st_size;

    while (current_file_position <= file_size)
    {
        read(tar, buffer, NAMELEN); // READ THE NAME

        if (buffer[0] != '\0') // CHECKS IF BUFFER ISN'T EMPTY TO PRINT HEADER NAME
            printf("%s\n", buffer);

        lseek(tar, 24, SEEK_CUR);   // LOOKS FOR THE SIZE OF THE FILE
        read(tar, buffer, SIZELEN); // READS THE SIZE OF THE FILE

        size = atoi(buffer);       // SIZE TO INT
        lseek(tar, 376, SEEK_CUR); // SEEKS THE END OF THE HEADER BLOCK

        if (size > block_size_octal)
        {

            size /= block_size_octal; // COUNTING HOW MANY BLOCKS ARE SAVED FOR CONTENT
            for (int i = 0; size - i > 1; i++)
                counter++;
        }
        current_file_position = lseek(tar, sizeof(buffer) * (counter), SEEK_CUR); // SEEKS THE NEXT HEADER AND SAVES THAT TO SEE IF WE ARRIVED TO THE END OF THE FILE

        counter = 1;
        index++;
    }

    close(tar);
}
