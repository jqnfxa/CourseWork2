#pragma once

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

enum OPERATION_TYPE {
	UNDEFINED = -1,
	DRAW_RECTANGLE = 0,
	DRAW_FRAME = 1,
	DRAW_CIRCLE = 2,
	ROTATE_IMAGE = 3
};

enum ROTATE_DIRECTION {
	INVALID = -1,
	LEFT = 0,
	RIGHT = 1
};

typedef struct Point {
	int x;
	int y;
} Point;
