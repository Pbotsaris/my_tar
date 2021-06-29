#include <fcntl.h>  
#include "my_tar.h"

int archive(header_t *header)
{

	char buff[] = "hello_world\n";

	int fd = open("test.tar", O_RDWR | O_CREAT);
	chmod("test.tar", S_IRUSR | S_IWUSR | S_IWGRP | S_IRGRP);

	if(fd == -1){
		return 1;
	}

		write(fd, header->prefix, strlen(header->prefix));
		write(fd, header->name, strlen(header->name));
		write(fd, header->mode, strlen(header->mode));


	if (close(fd) > 0)
		return 1;


	return 0;

}
