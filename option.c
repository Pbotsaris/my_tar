
typedef enum
{
    c,
    r,
    t,
    u,
    x,
    NONE
} option_t;

typedef enum
{
    TRUE,
    FALSE
} bool_f;

#define F_NOT_FOUND "my_tar: Refusing to read archive contents from terminal (missing -f option?)\n"
#define NULL_OPT "my_tar: Error is not recoverable: exiting now\n"

option_t check_option(int argc, char *argv[])
{
    int index = 0;

    option_t flag_opt = NONE;
    bool_f flag_bool = FALSE;

    while (argv[index] && flag_bool == FALSE)
    {
        if (argv[index] == '-')
        {
            if (argv[index][1] == 'c')
                flag_opt = c;
            if (argv[index][1] == 't')
                flag_opt = t;
            if (argv[index][1] == 'r')
                flag_opt = r;
            if (argv[index][1] == 'u')
                flag_opt = u;
            if (argv[index][1] == 'x')
                flag_opt = x;
            if (argv[index][2] == 'f' || argv[index][1] == 'f')
                flag_bool = TRUE;
        }
        index++;
    }

    if (flag_bool == FALSE)
    {
        printf(F_NOT_FOUND);
        flag_opt = NONE;
    }
    if (flag_opt == NONE)
        printf(NULL_OPT);

    return flag_opt;
}