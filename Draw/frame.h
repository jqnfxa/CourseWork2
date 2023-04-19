#pragma once

#include "../CLI/query_structures.h"
#include <stdbool.h>

void complete_frame_query(Matrix *matrix, FrameQuery *query);
void gen_simple_lines(Matrix **src, int32_t frame_width, int32_t color);
void gen_simple_circles(Matrix **src, int32_t frame_width, int32_t color);