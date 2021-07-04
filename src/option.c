#include "my_tar.h"

int search_dash(char *str)
{
	bool_t has_dash = FALSE;
	int index = 0;

	while(str[index] != '\0'){
		if(str[index] == '-')	
		{
			has_dash = TRUE;
			break;
		}
		// if not a space then not an option
		else if(str[index] != ' ')
			break;
		else
			index++;
	}
	return has_dash ? index : -1;
}

option_t select_option(char flag, option_t flag_opt)
{
	switch (flag)
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
			break;
		case 'x':
			flag_opt = x;
			break;
		default:
			flag_opt = ERROROPT;
	}
	return flag_opt;
}



option_t search_flag_f(option_t flag_opt, char *str)
{
	bool_t has_flag_f = FALSE;
	int pos = search_dash(str);

	if(pos >= 0)
	{
		int i = pos;
		while(str[i] != '\0'){

			if (str[i] == 'f'){
				has_flag_f = TRUE;
				break;
			}
			i++;
		}
	}
	else 
		return MISSING_F;

	return has_flag_f ? flag_opt :  MISSING_F;
}


void print_error(option_t flag_opt)
{
	if(flag_opt == MISSING_F)
		printf("%s", MISSING_F_ERR);

	if(flag_opt == ERROROPT)
		printf("%s", MISSING_OPT_ERR);

	if(flag_opt == NONE)
		printf("%s", NULL_OPT);
}





/*
 * =====================================================================================
 *
 *   CHECK OPTION																																										     
 *   																									                     													 
 *    - Parses trough argv for options and return the select options 
 *    - there is not default options. User must select one of the following otheise an error will be returned
 *    *** [-c] [-u] [-t] [-x]
 *    - Options must be accompanied by [-f] otherwise program will return an error.
 *    - ERROR: function will print error message but return error type to be handled outside this function
 *
 *    
 * =====================================================================================
 */

option_t check_option(char **argv)
{
	int index = 1;
	option_t flag_opt = NONE;

	while (argv[index])
	{
		if(flag_opt != NONE)
			break;
		// returns -1 if not found
		int pos = search_dash(argv[index]);
		if (pos >= 0) 
		{
			size_t len = strlen(argv[index]); 
			// options together: -cf, -uf 	
			if (len >= 2 && argv[index][pos + 2] == 'f'){
				flag_opt = select_option(argv[index][pos + 1], flag_opt);
			}
			else {
				// split options: -c -f	
				flag_opt = select_option(argv[index][pos + 1], flag_opt);
				if(flag_opt != ERROROPT){
					char *argv_temp = argv[index];
					flag_opt = search_flag_f(flag_opt, argv_temp + pos);
				}
			}
		}
		index++;
	}

	print_error(flag_opt);
	return flag_opt;
}

/*
 * =====================================================================================
 *
 *   SEARCH FLAG																																										     
 *   																									                     													 
 *    -  Searches for an specifc flag starting with a dash. E.G -d, -s.
 *
 *    
 * =====================================================================================
 */


bool_t search_flag(char **argv, char flag)
{
	int index = 1;
	bool_t found_flag = FALSE;

	while(argv[index]){
		int pos =	search_dash(argv[index]);
		if(pos >= 0 && argv[index][pos + 1] == flag)
			found_flag = TRUE;

		index++;
	}
	return found_flag;
}

int find_paths_start_index(char **argv)
{

	int index = 1;
	while(argv[index]){
		int pos =	search_dash(argv[index]);
		//  not an option
		if(pos < 0)
			break;

		index++;
	}
	return index;
}

bool_t validate_tar_extention(char *path)
{
	size_t len = strlen(path);
	bool_t is_valid = TRUE;
	int ext_index = 3;
	char ext[5] = ".tar";

	for(int path_index = len - 1; path_index >= (int)len - ext_index; path_index--)
	{
		if(path[path_index] != ext[ext_index])
			is_valid = FALSE;

		ext_index--;	
	}
	return is_valid;
}


