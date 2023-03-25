#include <getopt.h>
#include <string.h>
#include "types_parser.h"
#include "../ExceptionHandler/error_handler.h"
#include "rectangle_parser.h"

bool validate_rectangle(RectangleRequest *request)
{
	if(request == NULL)
	{
		return false;
	}
	// validate coordinates
	if((request->check_sum & 0b11) != 0b11)
	{
		log_error(MISSING_ARGUMENT, "--start | --end");
		return false;
	}
	if(request->left_up.x < 0 || request->left_up.x > request->right_bottom.x ||
	   request->left_up.y < 0 || request->left_up.y > request->right_bottom.y)
	{
		log_error(CONVERSATION_ERROR, "--start | --end");
		return false;
	}

	// validate color
	if((request->check_sum & (1 << 2)) != (1 << 2))
	{
		log_error(MISSING_ARGUMENT, "--color");
		return false;
	}
	if(!is_valid_rgb(request->color))
	{
		log_error(CONVERSATION_ERROR, "--color");
		return false;
	}

	// check if we have override width
	if((request->check_sum & (1 << 3)) != 0)
	{
		if(request->width <= 0)
		{
			log_error(CONVERSATION_ERROR, "--width");
			return false;
		}
	}
	else
	{
		request->width = 1;
	}

	// check if we have fill
	if((request->check_sum & (1 << 4)) != 0)
	{
		if(!is_valid_rgb(request->fill_color))
		{
			log_error(CONVERSATION_ERROR, "--fill");
			return false;
		}
	}

	return true;
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

	memset(request, 0, sizeof(RectangleRequest));

	int operation, operation_index = 0;
	while((operation = getopt_long(argc, argv, "s:e:w:c:f:n:", long_options, &operation_index)) != -1)
	{
		switch(operation)
		{
			case 's':
				if(!parse_point_values(optarg, &request->left_up, long_options[operation_index].name))
				{
					return false;
				}
				request->check_sum |= (1 << 0);
				break;
			case 'e':
				if(!parse_point_values(optarg, &request->right_bottom, long_options[operation_index].name))
				{
					return false;
				}
				request->check_sum |= (1 << 1);
				break;
			case 'w':
				if(!parse_int(optarg, &request->width, long_options[operation_index].name, 10))
				{
					return false;
				}
				request->check_sum |= (1 << 3);
				break;
			case 'c':
				if(!parse_color(optarg, &request->color, long_options[operation_index].name))
				{
					return false;
				}
				request->check_sum |= (1 << 2);
				break;
			case 'f':
				if(!parse_color(optarg, &request->fill_color, long_options[operation_index].name))
				{
					return false;
				}
				request->check_sum |= (1 << 4);
				break;
			case 'n':
				if(!parse_file_name(optarg, request->new_file, long_options[operation_index].name))
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
	if(!validate_rectangle(request))
	{
		return false;
	}

	strcpy(file_name, argv[optind]);
	return true;
}