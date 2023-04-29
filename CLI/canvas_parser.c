#include "../ExceptionHandler/logger.h"
#include "../Validator/validator.h"
#include "command_parser.h"
#include "types_parser.h"
#include <getopt.h>
#include <string.h>
#include <stdio.h>

bool parse_canvas_query(int32_t argc, char *argv[], char *file_name, CanvasQuery *query)
{
	if(query == NULL)
	{
		return false;
	}
	struct option long_options[] =
		{
			{"width", required_argument, NULL, 'w'},
			{"height", required_argument, NULL, 'h'},
			{"color", required_argument, NULL, 'c'},
			{"new", required_argument, NULL, 'n'},
			{0, 0, 0, 0}};

	int32_t operation, operation_index = 0;
	while((operation = getopt_long(argc, argv, "w:h:c:n:", long_options, &operation_index)) != -1)
	{
		switch(operation)
		{
			case 'h':
				if(!parse_int(optarg, &query->height, long_options[1].name, 10))
				{
					return false;
				}
				set_flags(&query->check_sum, HEIGHT);
				break;
			case 'w':
				if(!parse_int(optarg, &query->width, long_options[0].name, 10))
				{
					return false;
				}
				set_flags(&query->check_sum, WIDTH);
				break;
			case 'c':
				if(!parse_color(optarg, &query->color, long_options[2].name))
				{
					return false;
				}
				set_flags(&query->check_sum, COLOR);
				break;
			case 'n':
				if(!parse_file_name(optarg, query->new_file, long_options[3].name))
				{
					return false;
				}
				set_flags(&query->check_sum, NEW);
				break;
			case '?':
				free_query(CREATE_CANVAS, query);
				print_usage(optopt);
			default:
				break;
		}
	}

	if(!validate_canvas(query))
	{
		return false;
	}

	strcpy(file_name, query->new_file);
	return true;
}