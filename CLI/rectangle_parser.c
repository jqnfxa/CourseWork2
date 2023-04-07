#include "../ExceptionHandler/logger.h"
#include "../Validator/validator.h"
#include "command_parser.h"
#include "types_parser.h"
#include <getopt.h>
#include <stddef.h>
#include <string.h>

bool parse_rectangle_query(int32_t argc, char *argv[], char *file_name, RectangleQuery *query)
{
	if(query == NULL)
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

	memset(query, 0, sizeof(RectangleQuery));

	int operation, operation_index = 0;
	while((operation = getopt_long(argc, argv, "s:e:w:c:f:n:", long_options, &operation_index)) != -1)
	{
		switch(operation)
		{
			case 's':
				if(!parse_point_values(optarg, &query->area.left_up, long_options[operation_index].name))
				{
					return false;
				}
				set_flags(&query->check_sum, START_POINT);
				break;
			case 'e':
				if(!parse_point_values(optarg, &query->area.right_bottom, long_options[operation_index].name))
				{
					return false;
				}
				set_flags(&query->check_sum, END_POINT);
				break;
			case 'w':
				if(!parse_int(optarg, &query->width, long_options[operation_index].name, 10))
				{
					return false;
				}
				set_flags(&query->check_sum, WIDTH);
				break;
			case 'c':
				if(!parse_color(optarg, &query->color, long_options[operation_index].name))
				{
					return false;
				}
				set_flags(&query->check_sum, COLOR);
				break;
			case 'f':
				if(!parse_color(optarg, &query->fill_color, long_options[operation_index].name))
				{
					return false;
				}
				set_flags(&query->check_sum, FILL);
				break;
			case 'n':
				if(!parse_file_name(optarg, query->new_file, long_options[operation_index].name))
				{
					return false;
				}
				set_flags(&query->check_sum, NEW);
				break;
			case '?':
				print_usage(optopt);
			default:
				break;
		}
	}

	if(optind >= argc || !is_valid_bmp(argv[optind]))
	{
		log_error(TOO_FEW_ARGUMENTS, " for rectangle");
		return false;
	}
	if(!validate_rectangle(query))
	{
		log_error(INPUT_INVALID, " for rectangle");
		return false;
	}

	strcpy(file_name, argv[optind]);
	return true;
}