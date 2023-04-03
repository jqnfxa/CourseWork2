#pragma once

#include "../CLI/query_structures.h"
#include "../Geometry/structures.h"

void draw_circle(Matrix *matrix, CircleQuery *info);
void draw_circle_v1(Matrix *matrix, Point center, int32_t outer, int32_t inner, int32_t color);
void xLine(Matrix *matrix, int32_t x1, int32_t x2, int32_t y, int32_t color);
void yLine(Matrix *matrix, int32_t x, int32_t y1, int32_t y2, int32_t color);