#include "../include/my_tar.h"


void print_values(char *field, size_t len, int field_pos, int offset)
{
   char header_field[][20] = {"name", "mode", "uid", "gid", "size", "mtime", "chksum", "linkname", "typeflag", "magic", "version" , "uname", "gname", "devmajor", "devminor", "preflix"};

	char *temp = field;
	unsigned int sum = 0;
	printf("Field: %s -- length: %lu  -- offset: %d\n", header_field[field_pos], len, offset);

	for (int i = len - 1; i >= 0; i--){
		if(*temp != '\0')
		printf("|  %c  ", *temp);
		else
			printf("|  _  ");
		temp++;
	}
	printf("|\n\n -- \n\n");
}


void debug_header(char *path)
{

	header_t *header; 
	struct stat stats;

	if ( stat(path, &stats) == 0)
			header = create_header(path, stats);
	else
		return;



	int *bytes_offset = create_bytes_offset();

	char *temp = header->name;
	int i;
	int offset = 0;

	for (i = 0; i < BYTOFFLEN - 1; ++i) {
			temp += bytes_offset[i];
			offset += bytes_offset[i];
			print_values(temp, bytes_offset[i+ 1], i, offset);
	}

	free(bytes_offset);
	free(header);

}

