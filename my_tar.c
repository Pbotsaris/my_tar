/*
 * =====================================================================================
 *
 *       Filename:  my_tar.c
 *
 *    Description:  A naive implementation with the infamous tar
 *
 *        Version:  0.1
 *        Created:  06/22/2021 16:05:36
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Khalil Masri, Pedro Botsaris
 *   Organization:  ELU
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
	
		for (int i = 0; i < argc; ++ i) 
				printf("%s\n", argv[i]);	
		
		return 0;
}

