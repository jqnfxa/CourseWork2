#include "../../interfaces/CLI/types_parser.h"
#include "../../interfaces/ExceptionHandler/error_handler.h"
#include <errno.h>
#include <stdlib.h>
#include <string.h>

bool match_flags(int value, int flags)
{
	return (value & flags) == flags;
}

void set_flags(int *value, int flags)
{
	if(value == NULL)
	{
		return;
	}
	*value |= flags;
}

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
	// TODO may be let input like: 1,2,2,2,... i.e. parse only 2 first values?
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
		log_error(CONVERSATION_ERROR, option_name);
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
		log_error(CONVERSATION_ERROR, "--new");
		return false;
	}

	strcpy(name, argument);
	return true;
}

bool parse_color(const char *argument, int *color, const char *option_name)
{
	if(argument == NULL)
	{
		return false;
	}

	if(*argument != '#')
	{
		return false;
	}
	return parse_int(argument + 1, color, option_name, 16);
}

bool match_postfix(const char *filename, const char *needle)
{
	if(filename == NULL)
	{
		return false;
	}

	if(strlen(filename) < strlen(needle))
	{
		return false;
	}

	// calculate offset to match
	size_t offset = strlen(filename) - strlen(needle);

	return !strcmp(filename + offset, needle);
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

	return match_postfix(file_name, ".bmp");
}

bool is_valid_rgb(int color)
{
	if(color < 0 || color > 0xffffff)
	{
		return false;
	}
	return true;
}

bool validate_area(Point *left_up, Point *right_bottom)
{
	if(left_up == NULL || right_bottom == NULL)
	{
		return false;
	}
	if(left_up->x < 0 || left_up->y < 0 || left_up->x > right_bottom->x || left_up->y > right_bottom->y)
	{
		return false;
	}

	return true;
}

bool is_square_area(Point *left_up, Point *right_bottom)
{
	if(!validate_area(left_up, right_bottom))
	{
		return false;
	}

	return (right_bottom->x - left_up->x) == (right_bottom->y - left_up->y);
}