#include "../../interfaces/CLI/rectangle_parser.h"
#include "../../interfaces/ExceptionHandler/error_handler.h"
#include "../../interfaces/CLI/types_parser.h"
#include <getopt.h>
#include <string.h>
#include <stdio.h>

bool validate_rectangle(RectangleRequest *request)
{
	if(request == NULL)
	{
		return false;
	}
	// validate coordinates
	if(!match_flags(request->check_sum, START_POINT | END_POINT))
	{
		log_error(MISSING_ARGUMENT, "--start | --end");
		return false;
	}
	if(!validate_area(&request->left_up, &request->right_bottom))
	{
		log_error(CONVERSATION_ERROR, "--start | --end");
		return false;
	}

	// validate color
	if(!match_flags(request->check_sum, COLOR))
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
	if(match_flags(request->check_sum, WIDTH))
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
	if(match_flags(request->check_sum, FILL))
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
			{0, 0, 0, 0}};

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
				set_flags(&request->check_sum, START_POINT);
				break;
			case 'e':
				if(!parse_point_values(optarg, &request->right_bottom, long_options[operation_index].name))
				{
					return false;
				}
				set_flags(&request->check_sum, END_POINT);
				break;
			case 'w':
				if(!parse_int(optarg, &request->width, long_options[operation_index].name, 10))
				{
					return false;
				}
				set_flags(&request->check_sum, WIDTH);
				break;
			case 'c':
				if(!parse_color(optarg, &request->color, long_options[operation_index].name))
				{
					return false;
				}
				set_flags(&request->check_sum, COLOR);
				break;
			case 'f':
				if(!parse_color(optarg, &request->fill_color, long_options[operation_index].name))
				{
					return false;
				}
				set_flags(&request->check_sum, FILL);
				break;
			case 'n':
				if(!parse_file_name(optarg, request->new_file, long_options[operation_index].name))
				{
					return false;
				}
				set_flags(&request->check_sum, NEW);
				break;
			case '?':
				fprintf(stderr, "unknown is: %s\n", optarg);
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