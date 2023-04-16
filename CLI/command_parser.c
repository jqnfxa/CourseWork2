#include "command_parser.h"
#include "../Draw/Shapes/circle.h"
#include "../Draw/Shapes/polygon.h"
#include "../Draw/Shapes/rectangle.h"
#include "../Draw/frame.h"
#include "../Draw/rotate.h"
#include "../ExceptionHandler/logger.h"
#include "../Validator/validator.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

bool (*QueryFunctions[6])(int32_t argc, char *argv[], char *file_name, void *query) = {
	(bool (*)(int32_t, char **, char *, void *))parse_rectangle_query,
	(bool (*)(int32_t, char **, char *, void *))parse_frame_query,
	(bool (*)(int32_t, char **, char *, void *))parse_circle_query,
	(bool (*)(int32_t, char **, char *, void *))parse_line_query,
	(bool (*)(int32_t, char **, char *, void *))parse_polygon_query,
	(bool (*)(int32_t, char **, char *, void *))parse_rotate_query,
};

bool (*TaskFunctions[6])(Matrix *matrix, void *query) = {
	(bool (*)(Matrix *, void *))draw_rectangle,
	(bool (*)(Matrix *, void *))complete_frame_query,
	(bool (*)(Matrix *, void *))draw_circle,
	(bool (*)(Matrix *, void *))draw_line,
	(bool (*)(Matrix *, void *))draw_polygon,
	(bool (*)(Matrix *, void *))rotate_area};

void *get_query_structure(int32_t idx)
{
	void *query = NULL;
	switch(idx)
	{
		case DRAW_RECTANGLE: {
			query = calloc(1, sizeof(RectangleQuery));
			break;
		}
		case DRAW_FRAME: {
			query = calloc(1, sizeof(FrameQuery));
			break;
		}
		case DRAW_CIRCLE: {
			query = calloc(1, sizeof(CircleQuery));
			break;
		}
		case DRAW_LINE: {
			query = calloc(1, sizeof(LineQuery));
			break;
		}
		case DRAW_POLYGON: {
			query = calloc(1, sizeof(PolygonQuery));
			break;
		}
		case ROTATE_IMAGE: {

			query = calloc(1, sizeof(RotateQuery));
			break;
		}
		case UNDEFINED:
		default:
			break;
	}
	return query;
}

void get_file_to_unload(int32_t query_idx, void *query, char *file_name)
{
	if(query == NULL || file_name == NULL)
	{
		return;
	}
	switch(query_idx)
	{
		case DRAW_RECTANGLE: {
			if(match_flags((*(RectangleQuery *)query).check_sum, NEW))
			{
				strcpy(file_name, (*(RectangleQuery *)query).new_file);
			}
			break;
		}
		case DRAW_FRAME: {
			if(match_flags((*(FrameQuery *)query).check_sum, NEW))
			{
				strcpy(file_name, (*(FrameQuery *)query).new_file);
			}
			break;
		}
		case DRAW_CIRCLE: {
			if(match_flags((*(CircleQuery *)query).check_sum, NEW))
			{
				strcpy(file_name, (*(CircleQuery *)query).new_file);
			}
			break;
		}
		case DRAW_LINE: {
			if(match_flags((*(LineQuery *)query).check_sum, NEW))
			{
				strcpy(file_name, (*(LineQuery *)query).new_file);
			}
			break;
		}
		case DRAW_POLYGON: {
			if(match_flags((*(PolygonQuery *)query).check_sum, NEW))
			{
				strcpy(file_name, (*(PolygonQuery *)query).new_file);
			}
			break;
		}
		case ROTATE_IMAGE: {
			if(match_flags((*(RotateQuery *)query).check_sum, NEW))
			{
				strcpy(file_name, (*(RotateQuery *)query).new_file);
			}
			break;
		}
		case UNDEFINED:
		default:
			break;
	}
}

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
			{"line", no_argument, &operation_type, DRAW_LINE},
			{"polygon", no_argument, &operation_type, DRAW_POLYGON},
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