#pragma once
#include "../Declarations/structures.h"
#include <stdbool.h>

void set_pixel(Matrix *matrix, Point pixel, int color);
bool is_in_bound(Matrix *matrix, Point position);