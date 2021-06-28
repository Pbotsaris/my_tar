#include "my_tar.h"

option_t check_spam(option_t flag)
{
	option_t check_flag = NONE;

	if (flag == NONE)
		return check_flag;
	else
		return check_flag = ERROROPT;
}

bool_t check_f(bool_t flag_f, option_t flag_opt, char **format, int index)
{
	if (flag_opt == NONE)
		if (format[index][1] == 'f')
			return ERRORF;

	if (flag_opt != NONE && format[index][1] == 'f')
		return TRUE;

	if (flag_f == FALSE && format[index][2] == 'f')
		return TRUE;

	if (flag_f == TRUE)
		return ERRORF;

	return FALSE;
}

option_t error_handler(bool_t flag_f, option_t flag_opt)
{
	if (flag_f == FALSE)
	{
		printf(F_NOT_FOUND);
		flag_opt = ERROROPT;
	}
	if (flag_f == ERRORF)
	{
		printf(F_ERROR);
		flag_opt = ERROROPT;
	}
	if (flag_opt == NONE || flag_opt == ERROROPT)
	{
		printf(NULL_OPT);
	}

	return flag_opt;
}

option_t check_option(char **format)
{
	int index = 1;

	option_t flag_opt = NONE;
	bool_t flag_f = FALSE;

	while (format[index])
	{
		if (format[index][0] == '-' && (flag_opt = check_spam(flag_opt)) == NONE)
		{
			if (strlen(format[index]) == 2 || (strlen(format[index]) == 3 && format[index][2] == 'f'))
			{
				switch (format[index][1])
				{
				case 'c':
					flag_opt = c;
					break;
				case 't':
					flag_opt = t;
					break;
				case 'r':
					flag_opt = r;
					break;
				case 'u':
					flag_opt = u;
				case 'x':
					flag_opt = x;
					break;
				}
			}
			else
				flag_opt = ERROROPT;
		}

		flag_f = check_f(flag_f, flag_opt, format, index);

		index++;
	}

	flag_opt = error_handler(flag_f, flag_opt);

	return flag_opt;
}
