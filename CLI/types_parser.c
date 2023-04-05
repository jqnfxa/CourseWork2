#include "types_parser.h"
#include "../Validator/validator.h"
#include "../ExceptionHandler/logger.h"
#include <errno.h>
#include <stdlib.h>
#include <string.h>

bool parse_point_values(const char *argument, Point *point, const char *option_name)
{
	if(argument == NULL)
	{
		log_error(MISSING_ARGUMENT, option_name);
		return false;
	}

	char *copy = (char *)malloc((strlen(argument) + 1) * sizeof(char));

	if(copy == NULL)
	{
		log_error(OUT_OF_MEMORY, option_name);
		return false;
	}

	strcpy(copy, argument);

	int parsed[2] = {0, 0};
	int i = 0;
	char *token = NULL;
	for(token = strtok(copy, ",");
		i < 2 && token != NULL;
		token = strtok(NULL, ","), ++i)
	{
		if(!parse_int(token, parsed + i, option_name, 10))
		{
			free(copy);
			return false;
		}
	}
	//TODO may be let input like: 1,2,2,2,... i.e. parse only 2 first values?
	if(token != NULL || i != 2)
	{
		log_error(CONVERSATION, option_name);

		free(copy);
		return false;
	}

	point->x = parsed[0];
	point->y = parsed[1];

	free(copy);
	return true;
}

bool parse_int(const char *argument, int *val, const char *option_name, int base)
{
	if(val == NULL)
	{
		return false;
	}
	if(argument == NULL)
	{
		log_error(MISSING_ARGUMENT, option_name);
		return false;
	}
	*val = (int)strtol(argument, NULL, base);

	if(errno == EINVAL || errno == ERANGE)
	{
		log_error(CONVERSATION, option_name);
		return false;
	}

	return true;
}

bool parse_file_name(const char *argument, char *name, const char *option_name)
{
	if(name == NULL)
	{
		return false;
	}
	if(argument == NULL)
	{
		log_error(MISSING_ARGUMENT, option_name);
		return false;
	}
	if(!is_valid_bmp(argument))
	{
		log_error(CONVERSATION, "--new");
		return false;
	}

	strcpy(name, argument);
	return true;
}

bool parse_color(const char *argument, int *color, const char *option_name)
{
	if(argument == NULL || *argument != '#')
	{
		log_error(MISSING_ARGUMENT, option_name);
		return false;
	}

	return parse_int(argument + 1, color, option_name, 16);
}
