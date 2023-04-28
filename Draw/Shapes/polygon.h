#pragma once

#include "../image.h"
#include "../../CLI/query_structures.h"

void draw_line(Matrix *matrix, LineQuery *info);
void draw_polygon(Matrix *matrix, PolygonQuery *info);
int32_t draw_wide_line(Matrix *matrix, int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t color, int32_t width, int32_t danger_color);