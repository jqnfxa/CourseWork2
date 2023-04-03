#pragma once

#include "../Declarations/structures.h"
#include "../CLI/rectangle_parser.h"

void draw_rectangle(Matrix *matrix, RectangleRequest *info);
void draw_filled_rectangle(Matrix *matrix, Point left_up, Point right_bottom, int color);