#pragma once

#include "../BmpLib/bmp_24.h"
#include "../Geometry/matrix.h"

Matrix crop(Matrix *matrix, Area *area);
void paste(Matrix *dst, Matrix *src, Point *left_up);
void set_pixel(Matrix *matrix, Point *point, int32_t color);
bool is_in_bound(Matrix *matrix, Point *position);
int32_t rgb_to_int(RGB *color);
int32_t invert_color(int32_t color);
RGB int_to_rgb(int32_t color);