#pragma once

#include "structures.h"
#include <stdbool.h>

Matrix create(int32_t rows, int32_t columns);
void copy_additional(Matrix *dst, Matrix *src, const int *avoid_colors, int size);
void destroy(Matrix *matrix);

Matrix transpose(Matrix *matrix);
Matrix rotate_left(Matrix matrix);
Matrix rotate_right(Matrix matrix);
void rotate_180(Matrix *matrix);
void flip_horizontal(Matrix *matrix);
void flip_vertical(Matrix *matrix);

void swap(int32_t *lhs, int32_t *rhs);
