#pragma once

#include "../image.h"
#include "../../CLI/query_structures.h"

void draw_line(Matrix *matrix, LineQuery *info);
void draw_polygon(Matrix *matrix, PolygonQuery *info);