#include <getopt.h>
#include <string.h>
#include "../ExceptionHandler/error_handler.h"
#include "types_parser.h"
#include "frame_parser.h"

bool validate_frame(FrameRequest *request)
{
	if(request == NULL)
	{
		return false;
	}
	// check type
	if((request->check_sum & 0b1) != 0b1)
	{
		log_error(MISSING_ARGUMENT, "--type");
		return false;
	}
	// check color
	if((request->check_sum & 0b10) != 0b10)
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
	if((request->check_sum & 0b100) != 0)
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
				request->check_sum |= (1 << 0);
				break;
			case 'c':
				if(!parse_color(&request->color, long_options[operation_index].name))
				{
					return false;
				}
				request->check_sum |= (1 << 1);
				break;
			case 'w':
				if(!parse_int(&request->width, long_options[operation_index].name, 10))
				{
					return false;
				}
				request->check_sum |= (1 << 2);
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
	if(!validate_frame(request))
	{
		return false;
	}

	strcpy(file_name, argv[optind]);
	return true;
}
