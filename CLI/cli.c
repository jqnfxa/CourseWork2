//
// Created by shard on 3/23/23.
//

#include "cli.h"
#include "../ExceptionHandler/error_handler.h"
#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

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

bool parse_rectangle_request(int argc, char *argv[], char *file_name, RectangleRequest *request)
{
	if(request == NULL)
	{
		return false;
	}
	struct option long_options[] =
		{
			{"start", required_argument, NULL, 's'},
			{"end", required_argument, NULL, 'e'},
			{"width", required_argument, NULL, 'w'},
			{"color", required_argument, NULL, 'c'},
			{"fill", required_argument, NULL, 'f'},
			{"new", required_argument, NULL, 'n'},
			{0, 0, 0, 0}
		};

	int operation, operation_index = 0;
	while((operation = getopt_long(argc, argv, "s:e:w:c:f:n:", long_options, &operation_index)) != -1)
	{
		switch(operation)
		{
			case 's':
				if(!parse_point_values(&request->left_up, long_options[operation_index].name))
				{
					return false;
				}
				break;
			case 'e':
				if(!parse_point_values(&request->right_bottom, long_options[operation_index].name))
				{
					return false;
				}
				break;
			case 'w':
				if(!parse_int(&request->width, long_options[operation_index].name, 10))
				{
					return false;
				}
				break;
			case 'c':
				if(!parse_color(&request->color, long_options[operation_index].name))
				{
					return false;
				}
				break;
			case 'f':
				if(!parse_color(&request->fill_color, long_options[operation_index].name))
				{
					return false;
				}
				break;
			case 'n':
				if(!parse_file_name((char **)&request->new_file, long_options[operation_index].name))
				{
					return false;
				}
				break;
			case '?':
				usage(optopt);
			default:
				break;
		}
	}

	if(optind >= argc || !is_valid_bmp(argv[optind]))
	{
		return false;
	}

	strcpy(file_name, argv[optind]);
	return true;
}

bool parse_circle_request(int argc, char *argv[], char *file_name, CircleRequest *request)
{
	if(request == NULL)
	{
		return false;
	}
	struct option long_options[] =
		{
			{"point", required_argument, NULL, 'p'},
			{"radius", required_argument, NULL, 'r'},
			{"start", required_argument, NULL, 's'},
			{"end", required_argument, NULL, 'e'},
			{"width", required_argument, NULL, 'w'},
			{"color", required_argument, NULL, 'c'},
			{"fill", required_argument, NULL, 'f'},
			{"new", required_argument, NULL, 'n'},
			{0, 0, 0, 0}
		};

	int operation, operation_index = 0;
	while((operation = getopt_long(argc, argv, "p:r:s:e:w:c:f:n:", long_options, &operation_index)) != -1)
	{
		switch(operation)
		{
			case 'r':
				if(!parse_int(&request->radius, long_options[operation_index].name, 10))
				{
					return false;
				}
				break;
			case 'p':
				if(!parse_point_values(&request->center, long_options[operation_index].name))
				{
					return false;
				}
				break;
			case 's':
				if(!parse_point_values(&request->left_up, long_options[operation_index].name))
				{
					return false;
				}
				break;
			case 'e':
				if(!parse_point_values(&request->right_bottom, long_options[operation_index].name))
				{
					return false;
				}
				break;
			case 'w':
				if(!parse_int(&request->width, long_options[operation_index].name, 10))
				{
					return false;
				}
				break;
			case 'c':
				if(!parse_color(&request->color, long_options[operation_index].name))
				{
					return false;
				}
				break;
			case 'f':
				if(!parse_color(&request->fill_color, long_options[operation_index].name))
				{
					return false;
				}
				break;
			case 'n':
				if(!parse_file_name((char **)&request->new_file, long_options[operation_index].name))
				{
					return false;
				}
				break;
			case '?':
				usage(optopt);
			default:
				break;
		}
	}

	if(optind >= argc || !is_valid_bmp(argv[optind]))
	{
		return false;
	}

	strcpy(file_name, argv[optind]);
	return true;
}

bool parse_frame_request(int argc, char *argv[], char *file_name, FrameRequest *request)
{
	if(request == NULL)
	{
		return false;
	}
	struct option long_options[] =
		{
			{"type", required_argument, NULL, 't'},
			{"color", required_argument, NULL, 'c'},
			{"width", required_argument, NULL, 'w'},
			{"new", required_argument, NULL, 'n'},
			{0, 0, 0, 0}
		};

	int operation, operation_index = 0;
	while((operation = getopt_long(argc, argv, "t:c:w:n:", long_options, &operation_index)) != -1)
	{
		switch(operation)
		{
			case 't':
				if(!parse_int(&request->type, long_options[operation_index].name, 10))
				{
					return false;
				}
				break;
			case 'c':
				if(!parse_color(&request->color, long_options[operation_index].name))
				{
					return false;
				}
				break;
			case 'w':
				if(!parse_int(&request->width, long_options[operation_index].name, 10))
				{
					return false;
				}
				break;
			case 'n':
				if(!parse_file_name((char **)&request->new_file, long_options[operation_index].name))
				{
					return false;
				}
				break;
			case '?':
				usage(optopt);
			default:
				break;
		}
	}

	if(optind >= argc || !is_valid_bmp(argv[optind]))
	{
		return false;
	}

	strcpy(file_name, argv[optind]);
	return true;
}

bool parse_rotate_request(int argc, char *argv[], char *file_name, RotateRequest *request)
{
	if(request == NULL)
	{
		return false;
	}
	struct option long_options[] =
		{
			{"direction", required_argument, NULL, 'd'},
			{"angle", required_argument, NULL, 'a'},
			{"start", required_argument, NULL, 's'},
			{"end", required_argument, NULL, 'e'},
			{"new", required_argument, NULL, 'n'},
			{0, 0, 0, 0}
		};

	int operation, operation_index = 0;
	while((operation = getopt_long(argc, argv, "d:a:s:e:n:", long_options, &operation_index)) != -1)
	{
		switch(operation)
		{
			case 'd':
				if(optarg == NULL)
				{
					return false;
				}
				request->direction = (!strcmp(optarg, "left") ? LEFT : (!strcmp(optarg, "right") ? RIGHT : INVALID));
				break;
			case 'a':
				if(!parse_int(&request->angle, long_options[operation_index].name, 10))
				{
					return false;
				}
				break;
			case 's':
				if(!parse_point_values(&request->left_up, long_options[operation_index].name))
				{
					return false;
				}
				break;
			case 'e':
				if(!parse_point_values(&request->right_bottom, long_options[operation_index].name))
				{
					return false;
				}
				break;
			case 'n':
				if(!parse_file_name((char **)&request->new_file, long_options[operation_index].name))
				{
					return false;
				}
				break;
			case '?':
				usage(optopt);
			default:
				break;
		}
	}

	if(optind >= argc || !is_valid_bmp(argv[optind]))
	{
		return false;
	}

	strcpy(file_name, argv[optind]);
	return true;
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

enum OPERATION_TYPE parse_user_command(int argc, char *argv[])
{
	if(argc == 1)
	{
		usage(optopt);
	}
	int operation_type = UNDEFINED;

	struct option long_options[] =
	{
			{"rectangle", no_argument, &operation_type, DRAW_RECTANGLE},
			{"circle", no_argument, &operation_type, DRAW_CIRCLE},
			{"frame", no_argument, &operation_type, DRAW_FRAME},
			{"rotate", no_argument, &operation_type, ROTATE_IMAGE},
			{"help", no_argument, NULL, 'h'},
			{0, 0, 0, 0}
	};

	int operation, operation_index = 0;
	if((operation = getopt_long(argc, argv, "h", long_options, &operation_index)) != -1)
	{
		switch(operation)
		{
			case 'h':
			case '?':
				usage(optopt);
			default:
				break;
		}
	}
	return operation_type;
}