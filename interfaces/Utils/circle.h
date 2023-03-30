#pragma once
#include "../../interfaces/Declarations/structures.h"
#include "../../interfaces/CLI/circle_parser.h"

void draw_circle(Matrix *matrix, CircleRequest *info);
void draw_circle_v1(Matrix *matrix, Point center, int outer, int inner, int color);
void xLine(Matrix *matrix, int x1, int x2, int y, int color);
void yLine(Matrix *matrix, int x, int y1, int y2, int color);