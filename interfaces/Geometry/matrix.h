#pragma once
#include "../Declarations/structures.h"
#include <stdbool.h>

void swap(int32_t *lhs, int32_t *rhs);
Matrix create(size_t n, size_t m);
Matrix crop(Matrix *matrix, Point left_up, Point right_bottom);
void paste(Matrix *dst, Matrix *src, Point left_up);
void destroy(Matrix *matrix);
void set_pixel(Matrix *matrix, Point pixel, int color);
bool is_in_bound(Matrix *matrix, Point position);
Matrix transpose(Matrix *matrix);
Matrix rotate_left(Matrix matrix);
Matrix rotate_right(Matrix matrix);
void flip_horizontal(Matrix *matrix);
void flip_vertical(Matrix *matrix);
