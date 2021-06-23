#include <stdlib.h>
#include <stdio.h>






int  main(void)
{

		FILE *file;
		file = fopen("test.txt", "w");
		if(file == NULL)	
				return 1;
	
		fprintf(file, "4");
		fprintf(file, "6");
		fclose(file);


		return 0;
}
