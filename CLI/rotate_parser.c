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

	memset(query, 0, sizeof(RotateQuery));

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
				query->direction = (!strcmp(optarg, "left") ? LEFT : (!strcmp(optarg, "right") ? RIGHT : INVALID));
				set_flags(&query->check_sum, DIRECTION);
				break;
			case 'a':
				if(!parse_int(optarg, &query->angle, long_options[operation_index].name, 10))
				{
					return false;
				}
				set_flags(&query->check_sum, ANGLE);
				break;
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
		log_error(TOO_FEW_ARGUMENTS, " for rotate");
		return false;
	}
	if(!validate_rotate(query))
	{
		log_error(INPUT_INVALID, " for rotate");
		return false;
	}

	strcpy(file_name, argv[optind]);
	return true;
}