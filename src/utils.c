#include "my_tar.h"

int *create_bytes_offset(void)
{
int b[BYTOFFLEN] = {
		0, NAMELEN, MODELEN, UIDLEN, GIDLEN,
		SIZELEN, MTIMELEN, CHKSUMLEN, LINKNAMELEN,
		TYPFLAGLEN, TMAGLEN, TVERSLEN, UNAMELEN,
		GNAMELEN, DEVMAJORLEN, DEVMINORLEN,
		PREFIXLEN
	};
   int *bytes_offset = (int*)malloc(BYTOFFLEN * sizeof(int));


	 for (int i = 0; i < BYTOFFLEN; ++i) 
		 bytes_offset[i] = b[i];

	 return bytes_offset;
}


