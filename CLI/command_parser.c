#include "command_parser.h"
#include "../ExceptionHandler/logger.h"
#include <stddef.h>

enum OPERATION_TYPE parse_user_command(int32_t argc, char *argv[])
{
	if(argc == 1)
	{
		print_usage(optopt);
	}
	int operation_type = UNDEFINED;

	struct option long_options[] =
		{
			{"rectangle", no_argument, &operation_type, DRAW_RECTANGLE},
			{"circle", no_argument, &operation_type, DRAW_CIRCLE},
			{"frame", no_argument, &operation_type, DRAW_FRAME},
			{"rotate", no_argument, &operation_type, ROTATE_IMAGE},
			{"help", no_argument, NULL, 'h'},
			{0, 0, 0, 0}};

	int32_t operation, operation_index = 0;
	if((operation = getopt_long(argc, argv, "h", long_options, &operation_index)) != -1)
	{
		switch(operation)
		{
			case 'h':
			case '?':
				print_usage(optopt);
			default:
				break;
		}
	}
	return operation_type;
}