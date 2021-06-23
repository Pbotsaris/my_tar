#include "string_handling.c"
#include <stdio.h>
#include <stdlib.h>

typedef enum
{
    c,
    r,
    t,
    u,
    x,
    NONE,
    ERROROPT
} option_t;

typedef enum
{
    TRUE,
    FALSE,
    ERRORF
} bool_f;

#define F_NOT_FOUND "my_tar: Refusing to read archive contents from terminal (missing -f option?)\n"
#define F_ERROR "You must specify one of the the following options -c -r -t -u -x\n"
#define NULL_OPT "my_tar: Error is not recoverable: exiting now\n"
