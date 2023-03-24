#pragma once
#include <stdbool.h>
#include <getopt.h>

enum OPERATION_TYPE
{
	UNDEFINED = -1,
	DRAW_RECTANGLE = 0,
	DRAW_FRAME = 1,
	DRAW_CIRCLE = 2,
	ROTATE_IMAGE = 3
};

enum ROTATE_DIRECTION
{
	INVALID = -1,
	LEFT = 0,
	RIGHT = 1
};

typedef struct Point
{
	int x;
	int y;
}Point;

typedef struct RectangleRequest
{
	Point left_up;
	Point right_bottom;

	int width;
	int color;
	int fill_color;
	char new_file[256];
}
RectangleRequest;

typedef struct CircleRequest
{
	Point left_up;
	Point right_bottom;

	Point center;
	int radius;

	int width;
	int color;
	int fill_color;
	char new_file[256];
}
CircleRequest;

typedef struct FrameRequest
{
	int type;
	int color;
	int width;
	char new_file[256];
}
FrameRequest;

typedef struct RotateRequest
{
	Point left_up;
	Point right_bottom;

	enum ROTATE_DIRECTION direction;
	int angle;
	char new_file[256];
}
RotateRequest;

bool parse_point_values(Point *point, const char *option_name);
bool parse_int(int *val, const char *option_name, int base);
bool parse_color(int *color, const char *option_name);
bool parse_file_name(char **name, const char *option_name);

enum OPERATION_TYPE parse_user_command(int argc, char *argv[]);
bool parse_rectangle_request(int argc, char *argv[], char *file_name, RectangleRequest *request);
bool parse_circle_request(int argc, char *argv[], char *file_name, CircleRequest *request);
bool parse_frame_request(int argc, char *argv[], char *file_name, FrameRequest *request);
bool parse_rotate_request(int argc, char *argv[], char *file_name, RotateRequest *request);

bool is_valid_bmp(const char *file_name);

const char *to_str(enum OPERATION_TYPE operation);