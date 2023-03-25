#pragma once

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
