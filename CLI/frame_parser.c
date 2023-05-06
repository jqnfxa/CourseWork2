#include "../ExceptionHandler/logger.h"
#include "../Validator/validator.h"
#include "command_parser.h"
#include "types_parser.h"
#include <stddef.h>
#include <string.h>

bool parse_frame_query(int32_t argc, char *argv[], char *file_name, FrameQuery *query)
{
	if(query == NULL)
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

	int32_t operation, operation_index = 0;
	while((operation = getopt_long(argc, argv, "t:c:w:n:", long_options, &operation_index)) != -1)
	{
		switch(operation)
		{
			case 't':
				if(!parse_int(optarg, &query->type, long_options[0].name, 10))
				{
					return false;
				}
				set_flags(&query->check_sum, FRAME_TYPE);
				break;
			case 'c':
				if(!parse_color(optarg, &query->color, long_options[1].name))
				{
					return false;
				}
				set_flags(&query->check_sum, COLOR);
				break;
			case 'w':
				if(!parse_int(optarg, &query->width, long_options[2].name, 10))
				{
					return false;
				}
				set_flags(&query->check_sum, WIDTH);
				break;
			case 'n':
				if(!parse_file_name(optarg, query->new_file, long_options[3].name))
				{
					return false;
				}
				set_flags(&query->check_sum, NEW);
				break;
			case '?':
				free_query(DRAW_FRAME, query);
				print_usage(optopt);
			default:
				break;
		}
	}

	if(optind >= argc)
	{
		log_error(WRONG_ARGUMENT_NUMBER, "for frame");
		return false;
	}
	if(!is_valid_bmp_name(argv[optind]))
	{
		log_error(FILE_OPEN, argv[optind]);
		return false;
	}
	if(!validate_frame(query))
	{
		return false;
	}

	strcpy(file_name, argv[optind]);
	return true;
}