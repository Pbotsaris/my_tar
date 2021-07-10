#include "../include/my_tar.h"
#include "../include/messages.h"
#include "../include/header.h"

header_t *get_header(int tar)
{
    header_t *header = malloc(sizeof(header_t));
    char temp;
    read(tar, header->name, NAMELEN);
    read(tar, header->mode, MODELEN);
    read(tar, header->uid, UIDLEN);
    read(tar, header->gid, GIDLEN);
    read(tar, header->size, SIZELEN);
    read(tar, header->mtime, MTIMELEN);
    read(tar, header->chksum, CHKSUMLEN);
    read(tar, &temp, TYPFLAGLEN);
    header->typeflag = temp;
    read(tar, header->linkname, LINKNAMELEN);
    read(tar, header->magic, TMAGLEN);
    read(tar, header->version, TVERSLEN);
    read(tar, header->uname, UNAMELEN);
    read(tar, header->gname, GNAMELEN);
    lseek(tar, ENDBLK, SEEK_CUR);

    return header;
}

int skip_content(header_t *header)
{

    int block_size_octal = decimal_to_octal(BLOCKSIZE),
        content_size = atoi(header->size),
        counter = 1;

    if (content_size > block_size_octal)
    {
        content_size /= block_size_octal;

        for (int i = 0; (content_size - i) >= 1; i++)
            counter++;
    }
    return BLOCKSIZE * counter;
}


void make_directory(char *path)
{
    struct stat stats;   
  if (stat (path, &stats) == 0)
     printf("The directory %s already exists. Unable to extract." , path);
  else
   mkdir(path, S_IWUSR | S_IXUSR | S_IRUSR | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH); 
}

void extract(int tar, int file_position, int end_file)
{
    while (file_position <= end_file)
    {
      header_t *header = malloc(sizeof(header_t));

        header = get_header(tar);

        if (header->name[0] != '\0')
            printf("%s\n", header->name);
        if (header->typeflag != DIRTYPE)
            file_position = lseek(tar, skip_content(header), SEEK_CUR);
        else
        {
            make_directory(header->name);
            file_position += BLOCKSIZE;
        }

        free(header);
    }

}

void list(int tar, int file_position, int end_file)
{

while (file_position <= end_file)
    {
      header_t *header = malloc(sizeof(header_t));

        header = get_header(tar);
        if (header->name[0] != '\0')
            printf("%s\n", header->name);
        if (header->typeflag != DIRTYPE)
            file_position = lseek(tar, skip_content(header), SEEK_CUR);
        else
            file_position += BLOCKSIZE;

        free(header);
    }
}

int list_or_extract(char *path, option_t options)
{

    int tar = open(path, O_RDWR),
        file_position = 0,
        end_file = (lseek(tar, 0, SEEK_END) - BLOCKSIZE);

    lseek(tar, 0, SEEK_SET);

    if(options == t)
      list(tar, file_position, end_file);

    if(options == x)
      extract(tar, file_position, end_file);
    
    close(tar);

    return 0;
}
