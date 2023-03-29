#pragma once

#include "../Declarations/structures.h"
#include "../CLI/rectangle_parser.h"

void draw_rectangle(Matrix *matrix, RectangleRequest *info);
void draw_vertical_line(Matrix *matrix, int y_1, int y_2, int x, int color);
void draw_horizontal_line(Matrix *matrix, int x_1, int x_2, int y, int color);