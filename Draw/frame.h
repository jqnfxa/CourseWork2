#pragma once

#include "../CLI/query_structures.h"
#include <stdbool.h>

void complete_frame_query(Matrix *matrix, FrameQuery *query);
void gen_simple_lines(Matrix **src, int frame_width, int color);
void gen_simple_circles(Matrix **src, int frame_width, int color);