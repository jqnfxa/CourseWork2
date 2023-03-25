#include <getopt.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "../ExceptionHandler/error_handler.h"
#include "types_parser.h"

bool parse_point_values(Point *point, const char *option_name)
{
	if(optarg == NULL)
	{
		log_error(MISSING_ARGUMENT, option_name);
		return false;
	}

	char *copy = (char *)malloc((strlen(optarg) + 1) * sizeof(char));
	strcpy(copy, optarg);

	int parsed[2] = {0, 0};
	int i = 0;
	char *token = NULL;
	for(token = strtok(copy, ","); i < 2 && token != NULL; token = strtok(NULL, ","), ++i)
	{
		if(!parse_int(parsed + i, option_name, 10))
		{
			free(copy);
			return false;
		}
	}
	if(token != NULL || i != 2)
	{
		log_error(CONVERSATION_ERROR, option_name);

		free(copy);
		return false;
	}

	point->x = parsed[0];
	point->y = parsed[1];

	free(copy);
	return true;
}

bool parse_int(int *val, const char *option_name, int base)
{
	if(val == NULL)
	{
		return false;
	}
	if(optarg == NULL)
	{
		log_error(MISSING_ARGUMENT, option_name);
		return false;
	}
	*val = (int)strtol(optarg, NULL, base);

	if(errno == EINVAL || errno == ERANGE)
	{
		log_error(CONVERSATION_ERROR, option_name);
		return false;
	}

	return true;
}

bool parse_file_name(char **name, const char *option_name)
{
	if(name == NULL)
	{
		return false;
	}
	if(optarg == NULL)
	{
		log_error(MISSING_ARGUMENT, option_name);
		return false;
	}
	if(!is_valid_bmp(optarg))
	{
		return false;
	}

	strcpy(*name, optarg);
	return true;
}

bool parse_color(int *color, const char *option_name)
{
	return parse_int(color, option_name, 16);
}

bool is_valid_bmp(const char *file_name)
{
	if(file_name == NULL)
	{
		return false;
	}
	size_t file_len = strlen(file_name);
	if(file_len < 5 || file_len > 255)
	{
		return false;
	}
	const char cmp[] = "pmb.";
	for(int i = 0; i < strlen(cmp); ++i)
	{
		if(file_name[file_len - 1 - i] != cmp[i])
		{
			return false;
		}
	}
	return true;
}

bool is_valid_rgb(int color)
{
	if(color < 0 || color > 0xffffff)
	{
		return false;
	}
	return true;
}
