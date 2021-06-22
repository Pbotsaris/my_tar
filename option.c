typedef enum
{
    c,
    r,
    t,
    u,
    x,
    NONE,
    ERROR
} option_t;

typedef enum
{
    TRUE,
    FALSE
} bool_f;

#define F_NOT_FOUND "my_tar: Refusing to read archive contents from terminal (missing -f option?)\n"
#define NULL_OPT "my_tar: Error is not recoverable: exiting now\n"

option_t check_spam(option_t flag)
{
    option_t check_flag = NONE;

    if (flag == NONE)
        return check_flag;
    else
        return check_flag = ERROR;
}

option_t check_option(int argc, char **format)
{
    int index = 1;

    option_t flag_opt = NONE;
    bool_f flag_f = FALSE;

    while (format[index])
    {
        if (format[index][0] == '-')
        {
            switch (format[index][1])
            {
            case 'c':
                if ((flag_opt = check_spam(flag_opt)) == NONE)
                    flag_opt = c;
                break;
            case 't':
                if ((flag_opt = check_spam(flag_opt)) == NONE)
                    flag_opt = t;
                break;
            case 'r':
                if ((flag_opt = check_spam(flag_opt)) == NONE)
                    flag_opt = r;
                break;
            case 'u':
                if ((flag_opt = check_spam(flag_opt)) == NONE)
                    flag_opt = u;
            case 'x':
                if ((flag_opt = check_spam(flag_opt)) == NONE)
                    flag_opt = x;
                break;
            case 'f':
                flag_f = TRUE;
                break;
            }
        }

        if (format[index][2] == 'f' && flag_f != TRUE)
            flag_f = TRUE;

        index++;
    }

    if (flag_f == FALSE)
    {
        printf(F_NOT_FOUND);
        flag_opt = NONE;
    }
    if (flag_opt == NONE || flag_opt == ERROR)
        printf(NULL_OPT);

    return flag_opt;
}
