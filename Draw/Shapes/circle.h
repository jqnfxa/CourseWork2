#pragma once

#include "../../CLI/query_structures.h"
#include "../../Geometry/structures.h"
#include <stdbool.h>

void draw_circle(Matrix *matrix, CircleQuery *info);
void draw_circle_v1(Matrix *matrix, Point center, int32_t outer, int32_t inner, int32_t color);