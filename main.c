#include "interfaces/BMPLib/bmp_parser.h"
#include "interfaces/CLI/circle_parser.h"
#include "interfaces/CLI/cli.h"
#include "interfaces/CLI/frame_parser.h"
#include "interfaces/CLI/rectangle_parser.h"
#include "interfaces/CLI/rotate_parser.h"
#include "interfaces/Utils/rectangle.h"
#include "interfaces/CLI/types_parser.h"
#include "interfaces/Utils/circle.h"
#include "interfaces/Utils/rotate.h"
#include <stdio.h>
#include <time.h>

int main(int argc, char *argv[])
{
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
		case DRAW_RECTANGLE:
		{
			RectangleRequest request;
			if(parse_rectangle_request(argc, argv, file_to_process, &request))
			{
				BMP *a = load_image(file_to_process);

				if(a != NULL)
				{
					draw_rectangle(&a->matrix, &request);

					if(match_flags(request.check_sum, NEW))
					{
						unload_image(request.new_file, a);
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
		case DRAW_FRAME:
		{
			FrameRequest request;
			if(parse_frame_request(argc, argv, file_to_process, &request))
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
		case DRAW_CIRCLE:
		{
			CircleRequest request;
			if(parse_circle_request(argc, argv, file_to_process, &request))
			{
				BMP *a = load_image(file_to_process);

				if(a != NULL)
				{
					draw_circle(&a->matrix, &request);

					if(match_flags(request.check_sum, NEW))
					{
						unload_image(request.new_file, a);
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
		case ROTATE_IMAGE:
		{
			RotateRequest request;
			if(parse_rotate_request(argc, argv, file_to_process, &request))
			{
				BMP *a = load_image(file_to_process);

				if(a != NULL)
				{
					rotate_area(&a->matrix, &request);

					if(match_flags(request.check_sum, NEW))
					{
						unload_image(request.new_file, a);
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