#include <string.h>
#include <getopt.h>
#include "../ExceptionHandler/error_handler.h"
#include "types_parser.h"
#include "rotate_parser.h"

bool validate_rotate(RotateRequest *request)
{
	// validate coordinates
	if((request->check_sum & 0b11) != 0b11)
	{
		log_error(MISSING_ARGUMENT, "--start | --end");
		return false;
	}
	if(request->left_up.x < 0 || request->left_up.x < request->right_bottom.x ||
	   request->left_up.y < 0 || request->left_up.y < request->right_bottom.y)
	{
		log_error(CONVERSATION_ERROR, "--start | --end");
		return false;
	}
	// check angle
	if((request->check_sum & 0b100) != 0b100)
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
				request->check_sum |= (1 << 3);
				break;
			case 'a':
				if(!parse_int(&request->angle, long_options[operation_index].name, 10))
				{
					return false;
				}
				request->check_sum |= (1 << 2);
				break;
			case 's':
				if(!parse_point_values(&request->left_up, long_options[operation_index].name))
				{
					return false;
				}
				request->check_sum |= (1 << 0);
				break;
			case 'e':
				if(!parse_point_values(&request->right_bottom, long_options[operation_index].name))
				{
					return false;
				}
				request->check_sum |= (1 << 1);
				break;
			case 'n':
				if(!parse_file_name(request->new_file, long_options[operation_index].name))
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
	if(!validate_rotate(request))
	{
		return false;
	}

	strcpy(file_name, argv[optind]);
	return true;
}
