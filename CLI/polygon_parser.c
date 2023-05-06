#include "../ExceptionHandler/logger.h"
#include "../Validator/validator.h"
#include "command_parser.h"
#include "types_parser.h"
#include <getopt.h>
#include <stddef.h>
#include <string.h>

bool parse_polygon_query(int32_t argc, char *argv[], char *file_name, PolygonQuery *query)
{
	if(query == NULL)
	{
		return false;
	}
	struct option long_options[] =
		{
			{"points", required_argument, NULL, 'p'},
			{"width", required_argument, NULL, 'w'},
			{"color", required_argument, NULL, 'c'},
			{"fill", required_argument, NULL, 'f'},
			{"new", required_argument, NULL, 'n'},
			{0, 0, 0, 0}};

	int32_t operation, operation_index = 0;
	while((operation = getopt_long(argc, argv, "p:w:c:f:n:", long_options, &operation_index)) != -1)
	{
		switch(operation)
		{
			case 'p':
				if(!parse_points(optarg, &query->points, &query->points_count, long_options[0].name))
				{
					return false;
				}
				set_flags(&query->check_sum, START_POINT);
				break;
			case 'w':
				if(!parse_int(optarg, &query->width, long_options[1].name, 10))
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
			case 'f':
				if(!parse_color(optarg, &query->fill_color, long_options[3].name))
				{
					return false;
				}
				set_flags(&query->check_sum, FILL);
				break;
			case 'n':
				if(!parse_file_name(optarg, query->new_file, long_options[4].name))
				{
					return false;
				}
				set_flags(&query->check_sum, NEW);
				break;
			case '?':
				free_query(DRAW_POLYGON, query);
				print_usage(optopt);
			default:
				break;
		}
	}

	if(optind >= argc)
	{
		log_error(WRONG_ARGUMENT_NUMBER, "for polygon");
		return false;
	}
	if(!is_valid_bmp_name(argv[optind]))
	{
		log_error(FILE_OPEN, argv[optind]);
		return false;
	}
	if(!validate_polygon(query))
	{
		return false;
	}

	strcpy(file_name, argv[optind]);
	return true;
}