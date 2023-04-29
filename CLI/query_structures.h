#pragma once
#include "../Geometry/structures.h"

// flags used for checksums
#define START_POINT (1 << 0)
#define END_POINT (1 << 1)
#define COLOR (1 << 2)
#define WIDTH (1 << 3)
#define HEIGHT (1 << 11)
#define FILL (1 << 4)
#define NEW (1 << 5)
#define POINT (1 << 6)
#define RADIUS (1 << 7)
#define ANGLE (1 << 8)
#define DIRECTION (1 << 9)
#define FRAME_TYPE (1 << 10)
#define FRAME_TYPES_SUPPORTED 2

enum OPERATION_TYPE {
	UNDEFINED = -1,
	DRAW_RECTANGLE,
	DRAW_FRAME,
	DRAW_CIRCLE,
	DRAW_LINE,
	DRAW_POLYGON,
	ROTATE_IMAGE,
	CREATE_CANVAS,
	PRINT_INFO
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
	int32_t check_sum;

	char new_file[256];
} CircleQuery;

typedef struct FrameQuery {
	int32_t type;
	int32_t color;
	int32_t width;
	int32_t check_sum;

	char new_file[256];
} FrameQuery;

typedef struct RectangleQuery {
	Area area;

	int32_t width;
	int32_t color;
	int32_t fill_color;
	int32_t check_sum;

	char new_file[256];
} RectangleQuery;

typedef struct RotateQuery {
	Area area;

	enum ROTATE_DIRECTION direction;

	int32_t angle;
	int32_t check_sum;

	char new_file[256];
} RotateQuery;

typedef struct LineQuery {
	Point start;
	Point end;

	int32_t width;
	int32_t color;
	int32_t check_sum;

	char new_file[256];
} LineQuery;

typedef struct PolygonQuery {
	// points should be mallocated
	int32_t *points;
	int32_t points_count;

	int32_t width;
	int32_t color;
	int32_t fill_color;
	int32_t check_sum;

	char new_file[256];
} PolygonQuery;

typedef struct CanvasQuery {
	int32_t width;
	int32_t height;
	int32_t color;
	int32_t check_sum;

	char new_file[256];
} CanvasQuery;