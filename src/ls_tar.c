#include "../include/my_tar.h"
#include "../include/messages.h"
#include "../include/header.h"


header_t *get_header(int tar){
   // int tar = open(path, O_RDWR);
    header_t *header = malloc(sizeof(header_t));
    char temp;
    
    read(tar,header->name, NAMELEN);
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
    lseek(tar, ENDBLK + BLOCKSIZE, SEEK_CUR);

    return header;
}

int skip_content(header_t *header){
    
    int block_size_octal = decimal_to_octal(BLOCKSIZE),
        size = atoi(header->size),
        counter = 1;

   if(size > block_size_octal){
       size /= block_size_octal;
      
       for(int i = 0; (size - i) >= 1;i++)
            counter++;
    
   }

    return BLOCKSIZE * counter; 
}

int my_ls_tar(char *path){

    int tar = open(path, O_RDWR),
        current_file_position = 0,
        end_file = (lseek(tar,0,SEEK_END)-BLOCKSIZE);
    
    lseek(tar, 0, SEEK_SET);

    header_t *header = malloc(sizeof(header_t));

    while(current_file_position <= end_file){
        header = get_header(tar);
        if(header->name[0] != '\0')
            printf("%s\n", header->name);
        current_file_position = lseek(tar, skip_content(header), SEEK_CUR);        
        free(header);
    }
    close(tar);

    return 0;
}
//Needs to add directory
//int my_ls_tar(char *path)
//{
//    int tar = open(path, O_RDWR),
//        counter = 1,
//        current_file_position = 0,
//        end_file;
//
//    char buffer[BLOCKSIZE],
//        type[TYPFLAGLEN];
//
//    float size;
//
//    if (tar < 0)
//        return 1;
//
//    end_file = (lseek(tar, 0, SEEK_END) - BLOCKSIZE);
//    lseek(tar, 0, SEEK_SET);
//
//    while (current_file_position <= end_file)
//    {
//        read(tar, buffer, NAMELEN); // READ THE NAME
//
//        if (buffer[0] != '\0') // CHECKS IF BUFFER ISN'T EMPTY TO PRINT HEADER NAME
//            printf("%s\n", buffer);
//
//        lseek(tar, JMPSIZE, SEEK_CUR); // LOOKS FOR THE SIZE OF THE FILE
//
//        read(tar, buffer, SIZELEN);    // READS THE SIZE OF THE FILE
//        size = atoi(buffer);          // SIZE TO INT
//        lseek(tar, JMPFLAG, SEEK_CUR);
//        read(tar, type, 1);
//        printf("%s\n", type);
//        if(type[0] == REGTYPE || type[0] == AREGTYPE)
//            printf("Im here\n");
//        break;
//        if (size > block_size_octal)
//        {
//            size /= block_size_octal; // COUNTING HOW MANY BLOCKS ARE SAVED FOR CONTENT
//            for (int i = 0; (size - i) >= 1; i++)
//                counter++;
//        }
//        current_file_position = lseek(tar, sizeof(buffer) * (counter), SEEK_CUR); // SEEKS THE NEXT HEADER AND SAVES THAT TO SEE IF WE ARRIVED TO THE END OF THE FILE
//        counter = 1;
//    }
//
//    close(tar);
//}
