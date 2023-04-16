#pragma once

#include "../../Geometry/structures.h"
#include "../../CLI/query_structures.h"

void draw_rectangle(Matrix *matrix, RectangleQuery *info);
void draw_filled_rectangle(Matrix *matrix, Area *area, int32_t color);