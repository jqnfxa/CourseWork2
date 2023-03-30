#pragma once
#include <stddef.h>
#include <stdint.h>

#define START_POINT (1 << 0)
#define END_POINT   (1 << 1)
#define COLOR       (1 << 2)
#define WIDTH       (1 << 3)
#define FILL        (1 << 4)
#define NEW         (1 << 5)
#define POINT       (1 << 6)
#define RADIUS      (1 << 7)
#define ANGLE       (1 << 8)
#define DIRECTION   (1 << 9)
#define FRAME_TYPE  (1 << 10)

#define min(a, b) (a < b ? a : b)
#define max(a, b) (a > b ? a : b)

enum OPERATION_TYPE {
	UNDEFINED = -1,
	DRAW_RECTANGLE,
	DRAW_FRAME,
	DRAW_CIRCLE,
	ROTATE_IMAGE
};

enum ROTATE_DIRECTION {
	INVALID = -1,
	LEFT,
	RIGHT
};

typedef struct Point {
	int x;
	int y;
} Point;

typedef struct Matrix
{
	int32_t **grid;
	size_t height;
	size_t width;
} Matrix;
