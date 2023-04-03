#pragma once
#include "../Geometry/structures.h"

// flags used for checksums
#define START_POINT (1 << 0)
#define END_POINT (1 << 1)
#define COLOR (1 << 2)
#define WIDTH (1 << 3)
#define FILL (1 << 4)
#define NEW (1 << 5)
#define POINT (1 << 6)
#define RADIUS (1 << 7)
#define ANGLE (1 << 8)
#define DIRECTION (1 << 9)
#define FRAME_TYPE (1 << 10)

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

typedef struct CircleQuery {
	Area area;

	Point center;
	int32_t radius;

	int32_t width;
	int32_t color;
	int32_t fill_color;
	char new_file[256];
	int32_t check_sum;
} CircleQuery;

typedef struct FrameQuery {
	int32_t type;
	int32_t color;
	int32_t width;
	char new_file[256];
	int32_t check_sum;
} FrameQuery;

typedef struct RectangleQuery {
	Area area;

	int32_t width;
	int32_t color;
	int32_t fill_color;
	char new_file[256];
	int32_t check_sum;
} RectangleQuery;

typedef struct RotateQuery {
	Area area;

	enum ROTATE_DIRECTION direction;
	int32_t angle;
	char new_file[256];

	int32_t check_sum;
} RotateQuery;