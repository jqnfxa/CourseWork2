#pragma once

#include <stdint.h>

typedef struct Point {
	int32_t x;
	int32_t y;
} Point;

typedef struct Area {
	Point left_up;
	Point right_bottom;
} Area;

typedef struct Matrix {
	int32_t **grid;
	int32_t height;
	int32_t width;
} Matrix;
