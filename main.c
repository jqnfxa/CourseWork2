#include "interfaces/BMPLib/bmp_parser.h"
#include "interfaces/CLI/circle_parser.h"
#include "interfaces/CLI/cli.h"
#include "interfaces/CLI/frame_parser.h"
#include "interfaces/CLI/rectangle_parser.h"
#include "interfaces/CLI/rotate_parser.h"
#include <stdio.h>

int main(int argc, char *argv[])
{
	// test_bmp_read();
	// test_bmp_write();
	// printf("f %ld s %ld", sizeof(BITMAP_HEADER), sizeof(BITMAP_HEADER1));

	// test_frame_parser();
	//	test_rotate_parser();
	//	test_circle_parser();
	//	test_rectangle_parser();

	/*
	char file_to_process[256];

	// TODO functions for drawing, rotating ...

	switch(parse_user_command(argc, argv))
	{
		case DRAW_RECTANGLE:
		{
			RectangleRequest request;
			if(parse_rectangle_request(argc, argv, file_to_process, &request))
			{
				// do something
				printf("--rectangle valid\n");
			}
			else
			{
				printf("--rectangle invalid\n");
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
				printf("--frame invalid\n");
			}
			break;
		}
		case DRAW_CIRCLE:
		{
			CircleRequest request;
			if(parse_circle_request(argc, argv, file_to_process, &request))
			{
				// do something
				printf("--circle valid\n");
			}
			else
			{
				printf("--circle invalid\n");
			}
			break;
		}
		case ROTATE_IMAGE:
		{
			RotateRequest request;
			if(parse_rotate_request(argc, argv, file_to_process, &request))
			{
				printf("--rotate valid\n");
			}
			else
			{
				printf("--rotate invalid\n");
			}
			break;
		}
		case UNDEFINED:
		default:
			printf("--undefined--\n");
			break;
	}*/
	return 0;
}