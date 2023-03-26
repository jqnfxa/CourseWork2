#include "test_cli.h"
#include "../CLI/circle_parser.h"
#include "../CLI/frame_parser.h"
#include "../CLI/rectangle_parser.h"
#include "../CLI/rotate_parser.h"
#include <getopt.h>
#include <stdio.h>
#include <string.h>

void test_frame_parser()
{
	fprintf(stderr, "Test frame parser\n");
	// correct input
	{
		optind = 0;
		char *test1[] = {"--frame", "--type", "332", "--color", "#FF", "--width", "10", "-n", "new_image.bmp", "image.bmp"};
		char file_name[256];
		FrameRequest request;
		if(!parse_frame_request(10, test1, file_name, &request) || (request.color != 0xff || request.width != 10 || request.type != 332 || strcmp(request.new_file, "new_image.bmp") != 0))
		{
			fprintf(stderr, "Failed #1\n");
			return;
		}
	}
	// correct input but long variant
	{
		optind = 0;
		char *test2[] = {"--frame", "-t", "2", "--wid", "10", "-c", "#FF", "--n", "new_image.bmp", "image.bmp"};
		char file_name[256];
		FrameRequest request;
		if(!parse_frame_request(10, test2, file_name, &request) || (request.color != 0xff || request.width != 10 || request.type != 2 || strcmp(request.new_file, "new_image.bmp") != 0))
		{
			fprintf(stderr, "Failed #2\n");
			return;
		}
	}

	// correct input
	{
		optind = 0;
		char *test3[] = {"--frame", "--typ", "2", "-w", "10,15,16,17", "-c", "#FFFF", "image.bmp"};
		char file_name[256];
		FrameRequest request;
		if(!parse_frame_request(8, test3, file_name, &request) || (request.type != 2 || request.width != 10 || request.color != 0xFFFF || strcmp(file_name, "image.bmp") != 0))
		{
			fprintf(stderr, "Failed #3\n");
			return;
		}
	}

	fprintf(stderr, "passed\n");
}

void test_rotate_parser()
{
	fprintf(stderr, "Test rotate parser\n");
	// correct input
	{
		optind = 0;
		char *test1[] = {"--rotate",
						 "--direction", "left",
						 "--angle", "90",
						 "--start", "10,11",
						 "--end", "12,13",
						 "--new", "new_file.bmp",
						 "image.bmp"};
		char file_name[256];
		RotateRequest request;
		if(!parse_rotate_request(12, test1, file_name, &request) || (request.direction != LEFT || request.angle != 90 || request.left_up.x != 10 || request.left_up.y != 11 || request.right_bottom.x != 12 || request.right_bottom.y != 13 || strcmp(request.new_file, "new_file.bmp") != 0 || strcmp(file_name, "image.bmp") != 0))
		{
			fprintf(stderr, "Failed #1\n");
			return;
		}
	}
	// correct input
	{
		optind = 0;
		char *test2[] = {"--rotate",
						 "--start", "10,11",
						 "--end", "12,13",
						 "--angle", "180",
						 "--new", "new_file.bmp",
						 "image.bmp"};
		char file_name[256];
		RotateRequest request;
		if(!parse_rotate_request(12, test2, file_name, &request) || (request.direction != LEFT || request.angle != 180 || request.left_up.x != 10 || request.left_up.y != 11 || request.right_bottom.x != 12 || request.right_bottom.y != 13 || strcmp(request.new_file, "new_file.bmp") != 0 || strcmp(file_name, "image.bmp") != 0))
		{
			fprintf(stderr, "Failed #2\n");
			return;
		}
	}
	// correct input
	{
		optind = 0;
		char *test3[] = {"--rotate",
						 "--ne", "new_file.bmp",
						 "--en", "12,13",
						 "-s", "10,11",
						 "-a", "180",
						 "image.bmp"};
		char file_name[256];
		RotateRequest request;
		if(!parse_rotate_request(12, test3, file_name, &request) || (request.direction != LEFT || request.angle != 180 || request.left_up.x != 10 || request.left_up.y != 11 || request.right_bottom.x != 12 || request.right_bottom.y != 13 || strcmp(request.new_file, "new_file.bmp") != 0 || strcmp(file_name, "image.bmp") != 0))
		{
			fprintf(stderr, "Failed #3\n");
			return;
		}
	}

	fprintf(stderr, "passed\n");
}

void test_circle_parser()
{
	fprintf(stderr, "Test circle parser\n");
	// correct input
	{
		optind = 0;
		char *test1[] = {"--circle",
						 "-c", "#5f",
						 "--start", "10,11",
						 "--end", "12,13",
						 "--new", "new_file.bmp",
						 "image.bmp"};
		char file_name[256];
		CircleRequest request;
		if(!parse_circle_request(10, test1, file_name, &request) || (request.left_up.x != 10 || request.left_up.y != 11 || request.right_bottom.x != 12 || request.right_bottom.y != 13 || strcmp(request.new_file, "new_file.bmp") != 0 || strcmp(file_name, "image.bmp") != 0))
		{
			fprintf(stderr, "Failed #1\n");
			return;
		}
	}
	// correct input
	{
		optind = 0;
		char *test2[] = {"--circle",
						 "--wid", "14",
						 "-c", "#5f",
						 "--start", "10,11",
						 "--end", "12,13",
						 "--new", "new_file.bmp",
						 "image.bmp"};
		char file_name[256];
		CircleRequest request;
		if(!parse_circle_request(12, test2, file_name, &request) || (request.width != 14 || request.left_up.x != 10 || request.left_up.y != 11 || request.right_bottom.x != 12 || request.right_bottom.y != 13 || strcmp(request.new_file, "new_file.bmp") != 0 || strcmp(file_name, "image.bmp") != 0))
		{
			fprintf(stderr, "Failed #2\n");
			return;
		}
	}
	// incorrect input
	{
		optind = 0;
		char *test3[] = {"--circle",
						 "-p", "15, 16",
						 "-w", "14",
						 "-c", "#5f",
						 "--start", "10,11",
						 "--end", "12,13",
						 "--new", "new_file.bmp",
						 "image.bmp"};
		char file_name[256];
		CircleRequest request;
		if(parse_circle_request(14, test3, file_name, &request))
		{
			fprintf(stderr, "Failed #3\n");
			return;
		}
	}
	// incorrect input
	{
		optind = 0;
		char *test4[] = {"--circle",
						 "-w", "14",
						 "-c", "5f",
						 "--start", "10,12",
						 "--end", "12,13",
						 "image.bmp"};
		char file_name[256];
		CircleRequest request;
		if(parse_circle_request(10, test4, file_name, &request))
		{
			fprintf(stderr, "Failed #4\n");
			return;
		}
	}

	fprintf(stderr, "passed\n");
}

void test_rectangle_parser()
{
	fprintf(stderr, "Test rectangle parser\n");
	// correct input
	{
		optind = 0;
		char *test1[] = {"--rectangle",
						 "--start", "12,213",
						 "--end", "123,5345",
						 "-c", "#000ff0",
						 "--new", "new_file.bmp",
						 "image.bmp"};
		char file_name[256];
		RectangleRequest request;
		if(!parse_rectangle_request(10, test1, file_name, &request) || (request.left_up.x != 12 || request.left_up.y != 213 || request.width != 1 || request.right_bottom.x != 123 || request.right_bottom.y != 5345 || strcmp(request.new_file, "new_file.bmp") != 0 || strcmp(file_name, "image.bmp") != 0))
		{
			fprintf(stderr, "Failed #1\n");
			return;
		}
	}
	// correct input
	{
		optind = 0;
		char *test1[] = {"--rectangle",
						 "-w", "15",
						 "-s", "12,213",
						 "--en", "123,5345",
						 "-c", "#000ff0",
						 "--new", "new_file.bmp",
						 "image.bmp"};
		char file_name[256];
		RectangleRequest request;
		if(!parse_rectangle_request(12, test1, file_name, &request) || (request.left_up.x != 12 || request.left_up.y != 213 || request.width != 15 || request.right_bottom.x != 123 || request.right_bottom.y != 5345 || strcmp(request.new_file, "new_file.bmp") != 0 || strcmp(file_name, "image.bmp") != 0))
		{
			fprintf(stderr, "Failed #2\n");
			return;
		}
	}

	fprintf(stderr, "passed\n");
}