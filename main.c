#include "BmpLib/bmp_parser.h"
#include "CLI/command_parser.h"
#include "Draw/circle.h"
#include "Draw/rectangle.h"
#include "Draw/rotate.h"
#include "Validator/validator.h"
#include <stdio.h>

int main(int argc, char *argv[])
{
	/*BMP *bmp = create_image(4, 4, 0x0);
	unload_image("tiny.bmp", bmp);
	safe_free_bmp(&bmp);*/
	/*
	 *
	 * clock_t time;
	 * time = clock();
	 * fprintf(stderr, "Time used: %f ms\n", (clock() - time) / 1000.0);
	 *
	 */
	char file_to_process[256];

	switch(parse_user_command(argc, argv))
	{
		case DRAW_RECTANGLE: {
			RectangleQuery query;
			if(parse_rectangle_query(argc, argv, file_to_process, &query))
			{
				BMP *a = load_image(file_to_process);

				if(a != NULL)
				{
					draw_rectangle(&a->matrix, &query);

					if(match_flags(query.check_sum, NEW))
					{
						unload_image(query.new_file, a);
					}
					else
					{
						unload_image(file_to_process, a);
					}

					safe_free_bmp(&a);
				}
				else
				{
					fprintf(stderr, "output file not found\n");
				}
			}
			else
			{
				fprintf(stderr, "--rectangle request invalid\n");
			}
			break;
		}
		case DRAW_FRAME: {
			FrameQuery query;
			if(parse_frame_query(argc, argv, file_to_process, &query))
			{
				// do something
				printf("--frame valid\n");
			}
			else
			{
				printf("--frame request invalid\n");
			}
			break;
		}
		case DRAW_CIRCLE: {
			CircleQuery query;
			if(parse_circle_query(argc, argv, file_to_process, &query))
			{
				BMP *a = load_image(file_to_process);

				if(a != NULL)
				{
					draw_circle(&a->matrix, &query);

					if(match_flags(query.check_sum, NEW))
					{
						unload_image(query.new_file, a);
					}
					else
					{
						unload_image(file_to_process, a);
					}

					safe_free_bmp(&a);
				}
				else
				{
					fprintf(stderr, "output file not found\n");
				}
			}
			else
			{
				fprintf(stderr, "--circle request invalid\n");
			}
			break;
		}
		case ROTATE_IMAGE: {
			RotateQuery query;
			if(parse_rotate_query(argc, argv, file_to_process, &query))
			{
				BMP *a = load_image(file_to_process);

				if(a != NULL)
				{
					rotate_area(&a->matrix, &query);

					if(match_flags(query.check_sum, NEW))
					{
						unload_image(query.new_file, a);
					}
					else
					{
						unload_image(file_to_process, a);
					}

					safe_free_bmp(&a);
				}
				else
				{
					fprintf(stderr, "output file not found\n");
				}
			}
			else
			{
				printf("--rotate request invalid\n");
			}
			break;
		}
		case UNDEFINED:
		default:
			printf("--undefined--\n");
			break;
	}
	return 0;
}