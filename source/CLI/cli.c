//
// Created by shard on 3/23/23.
//

#include "../../interfaces/CLI/cli.h"
#include "../../interfaces/ExceptionHandler/error_handler.h"
#include <getopt.h>
#include <string.h>

enum OPERATION_TYPE parse_user_command(int argc, char *argv[])
{
	if(argc == 1)
	{
		usage(optopt);
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

	int operation, operation_index = 0;
	if((operation = getopt_long(argc, argv, "h", long_options, &operation_index)) != -1)
	{
		switch(operation)
		{
			case 'h':
			case '?':
				usage(optopt);
			default:
				break;
		}
	}
	return operation_type;
}