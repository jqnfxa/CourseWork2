#pragma once

#include "../CLI/query_structures.h"
#include "../Geometry/structures.h"
#include <stdbool.h>

#define max(a, b) (a > b ? a : b)
#define min(a, b) (a < b ? a : b)

bool match_postfix(const char *filename, const char *needle);
bool match_flags(int32_t value, int32_t flags);
void set_flags(int32_t *value, int32_t flags);

void shrink_to_fit(int32_t width, int32_t height, Area *area);
bool is_in_bound(Matrix *matrix, Point *position);
bool is_valid_rgb(int32_t color);
bool validate_area(Area *area);
bool is_square_area(Area *area);
bool validate_image_area(int32_t width, int32_t height, Area *area);
bool is_valid_bmp(const char *file_name);

bool validate_rectangle(RectangleQuery *query);
int16_t get_circle_type(CircleQuery *query);
bool validate_circle(CircleQuery *query);
bool validate_frame(FrameQuery *query);
bool validate_rotate(RotateQuery *query);
