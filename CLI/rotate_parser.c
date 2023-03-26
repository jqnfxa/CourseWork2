#include "rotate_parser.h"
#include "../ExceptionHandler/error_handler.h"
#include "types_parser.h"
#include <getopt.h>
#include <string.h>

bool validate_rotate(RotateRequest *request)
{
	// validate coordinates
	if(!match_flags(request->check_sum, START_POINT | END_POINT))
	{
		log_error(MISSING_ARGUMENT, "--start | --end");
		return false;
	}
	// TODO may be only square area??
	if(!validate_area(&request->left_up, &request->right_bottom))
	{
		log_error(CONVERSATION_ERROR, "--start | --end");
		return false;
	}
	// check angle
	if(!match_flags(request->check_sum, ANGLE))
	{
		log_error(MISSING_ARGUMENT, "--angle");
		return false;
	}
	else
	{
		if(request->angle != 90 && request->angle != 180 && request->angle != 270)
		{
			log_error(CONVERSATION_ERROR, "--angle");
			return false;
		}
	}

	// check direction
	if(request->direction == INVALID)
	{
		log_error(CONVERSATION_ERROR, "--direction");
		return false;
	}
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
			{0, 0, 0, 0}};

	memset(request, 0, sizeof(RotateRequest));

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
				set_flags(&request->check_sum, DIRECTION);
				break;
			case 'a':
				if(!parse_int(optarg, &request->angle, long_options[operation_index].name, 10))
				{
					return false;
				}
				set_flags(&request->check_sum, ANGLE);
				break;
			case 's':
				if(!parse_point_values(optarg, &request->left_up, long_options[operation_index].name))
				{
					return false;
				}
				set_flags(&request->check_sum, START_POINT);
				break;
			case 'e':
				if(!parse_point_values(optarg, &request->right_bottom, long_options[operation_index].name))
				{
					return false;
				}
				set_flags(&request->check_sum, END_POINT);
				break;
			case 'n':
				if(!parse_file_name(optarg, request->new_file, long_options[operation_index].name))
				{
					return false;
				}
				set_flags(&request->check_sum, NEW);
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
	if(!validate_rotate(request))
	{
		return false;
	}

	strcpy(file_name, argv[optind]);
	return true;
}
