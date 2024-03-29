#pragma once

#include "../../Geometry/matrix.h"
#include "../../CLI/query_structures.h"
#include "../../Geometry/structures.h"

void draw_circle(Matrix *matrix, CircleQuery *info);
void yLine(Matrix *matrix, int32_t x, int32_t y1, int32_t y2, int32_t color);
void xLine(Matrix *matrix, int32_t x1, int32_t x2, int32_t y, int32_t color);
void brezenham_ellipse(Matrix *matrix, int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t color, bool filled);
void mid_point_ellipse(Matrix *matrix, int32_t xc, int32_t yc, int32_t rx, int32_t ry, int32_t color, bool filled);
void draw_generic_ellipse(Matrix *matrix, int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t color, bool filled);
void draw_wide_ellipse(Matrix *matrix, int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t width, int32_t color);
