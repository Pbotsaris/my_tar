#ifndef MESSAGES_H
#define MESSAGES_H

/* OPTIONS */
#define MISSING_F_ERR "my_tar: Refusing to read archive contents from terminal (missing -f option?)\n"
#define MISSING_OPT_ERR "my_tar: You must specify one of the the following options -c -r -t -u -x\n"
#define NULL_OPT "my_tar: Error is not recoverable: exiting now\n"


/* HEADER */
#define EXC_NAME_SIZE "my_tar: Filename exceeds maximum length of 255\n"
#define FLAGTYPE_ERR "my_tar: File type not recognized. Archiving as regular file..."


/* FILE HANDLING */
#define STAT_ERR "my_tar: Unable to read file"
#define OPEN_FILE_ERR "my_tar: Error while openning file"
#define WRITE_FILE_ERR "my_tar: Error while writting file"
#define FILE_NOT_FOUND_ERR "my_tar: Error while writting file"

#endif


