#include "../../interfaces/CLI/frame_parser.h"
#include "../../interfaces/ExceptionHandler/error_handler.h"
#include "../../interfaces/CLI/types_parser.h"
#include <getopt.h>
#include <string.h>

bool validate_frame(FrameRequest *request)
{
	if(request == NULL)
	{
		return false;
	}
	// check type
	if(!match_flags(request->check_sum, FRAME_TYPE))
	{
		log_error(MISSING_ARGUMENT, "--type");
		return false;
	}
	// check color
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

	// check for width override
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
			{0, 0, 0, 0}};

	memset(request, 0, sizeof(FrameRequest));

	int operation, operation_index = 0;
	while((operation = getopt_long(argc, argv, "t:c:w:n:", long_options, &operation_index)) != -1)
	{
		switch(operation)
		{
			case 't':
				if(!parse_int(optarg, &request->type, long_options[operation_index].name, 10))
				{
					return false;
				}
				set_flags(&request->check_sum, FRAME_TYPE);
				break;
			case 'c':
				if(!parse_color(optarg, &request->color, long_options[operation_index].name))
				{
					return false;
				}
				set_flags(&request->check_sum, COLOR);
				break;
			case 'w':
				if(!parse_int(optarg, &request->width, long_options[operation_index].name, 10))
				{
					return false;
				}
				set_flags(&request->check_sum, WIDTH);
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
	if(!validate_frame(request))
	{
		return false;
	}

	strcpy(file_name, argv[optind]);
	return true;
}
