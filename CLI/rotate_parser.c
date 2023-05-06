#include "../ExceptionHandler/logger.h"
#include "../Validator/validator.h"
#include "command_parser.h"
#include "types_parser.h"
#include <stddef.h>
#include <string.h>

bool parse_rotate_query(int32_t argc, char *argv[], char *file_name, RotateQuery *query)
{
	if(query == NULL)
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

	int32_t operation, operation_index = 0;
	while((operation = getopt_long(argc, argv, "d:a:s:e:n:", long_options, &operation_index)) != -1)
	{
		switch(operation)
		{
			case 'd':
				if(optarg == NULL)
				{
					return false;
				}
				query->direction = (!strcmp(optarg, "left") ? LEFT : (!strcmp(optarg, "right") ? RIGHT : INVALID));
				set_flags(&query->check_sum, DIRECTION);
				break;
			case 'a':
				if(!parse_int(optarg, &query->angle, long_options[1].name, 10))
				{
					return false;
				}
				set_flags(&query->check_sum, ANGLE);
				break;
			case 's':
				if(!parse_point_values(optarg, &query->area.left_up, long_options[2].name))
				{
					return false;
				}
				set_flags(&query->check_sum, START_POINT);
				break;
			case 'e':
				if(!parse_point_values(optarg, &query->area.right_bottom, long_options[3].name))
				{
					return false;
				}
				set_flags(&query->check_sum, END_POINT);
				break;
			case 'n':
				if(!parse_file_name(optarg, query->new_file, long_options[4].name))
				{
					return false;
				}
				set_flags(&query->check_sum, NEW);
				break;
			case '?':
				free_query(ROTATE_IMAGE, query);
				print_usage(optopt);
			default:
				break;
		}
	}

	if(optind >= argc)
	{
		log_error(WRONG_ARGUMENT_NUMBER, "for rotate");
		return false;
	}
	if(!is_valid_bmp_name(argv[optind]))
	{
		log_error(FILE_OPEN, argv[optind]);
		return false;
	}
	if(!validate_rotate(query))
	{
		return false;
	}

	strcpy(file_name, argv[optind]);
	return true;
}