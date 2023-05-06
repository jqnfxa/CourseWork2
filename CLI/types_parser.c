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

	int32_t parsed[2] = {0, 0};
	int32_t i = 0;
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

bool parse_int(const char *argument, int32_t *val, const char *option_name, int32_t base)
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

	char *ptr = "\0";
	int64_t tmp = strtol(argument, &ptr, base);

	if(strcmp(ptr, "\0") != 0 || errno == EINVAL || errno == ERANGE || tmp > INT32_MAX || tmp < INT32_MIN)
	{
		log_error(CONVERSATION, option_name);
		return false;
	}

	*val = (int32_t)tmp;
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
	if(!is_valid_bmp_name(argument))
	{
		log_error(CONVERSATION, "--new");
		return false;
	}

	strcpy(name, argument);
	return true;
}

bool parse_color(const char *argument, int32_t *color, const char *option_name)
{
	if(argument == NULL || *argument != '#')
	{
		log_error(MISSING_ARGUMENT, option_name);
		return false;
	}

	return parse_int(argument + 1, color, option_name, 16);
}

bool parse_points(const char *argument, int32_t **arr, int32_t *count, const char *option_name)
{
	if(arr == NULL || count == NULL)
	{
		return false;
	}
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

	int32_t cap = 6;
	*arr = (int32_t *)malloc(sizeof(int32_t) * cap);
	*count = 0;

	if(*arr == NULL)
	{
		free(copy);
		log_error(OUT_OF_MEMORY, option_name);
		return false;
	}

	char *token = NULL;
	for(token = strtok(copy, ","); token != NULL;
		token = strtok(NULL, ","), ++(*count))
	{
		if(*count >= cap)
		{
			cap += 12;
			int32_t *tmp = (int32_t *)realloc(*arr, sizeof(int32_t) * cap);

			if(tmp == NULL)
			{
				free(*arr);
				*arr = NULL;
				free(copy);
				return false;
			}

			*arr = tmp;
		}
		if(!parse_int(token, *arr + *count, option_name, 10))
		{
			free(copy);
			free(*arr);
			*arr = NULL;
			return false;
		}
	}
	if(*count % 2 != 0)
	{
		log_error(CONVERSATION, option_name);

		free(*arr);
		*arr = NULL;
		free(copy);
		return false;
	}
	*count /= 2;

	if(*count < 3)
	{
		log_error(WRONG_ARGUMENT_NUMBER, "to form polygon");

		free(*arr);
		*arr = NULL;
		free(copy);
		return false;
	}

	free(copy);
	return true;
}