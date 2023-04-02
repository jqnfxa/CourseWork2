#pragma once
#include "../Declarations/structures.h"
#include <stdbool.h>

bool match_postfix(const char *filename, const char *needle);
bool match_flags(int value, int flags);
void set_flags(int *value, int flags);
bool parse_point_values(const char *argument, Point *point, const char *option_name);
bool parse_int(const char *argument, int *val, const char *option_name, int base);
bool parse_color(const char *argument, int *color, const char *option_name);
bool parse_file_name(const char *argument, char *name, const char *option_name);
bool is_valid_bmp(const char *file_name);
bool is_valid_rgb(int color);
bool validate_area(Point *left_up, Point *right_bottom);
bool is_square_area(Point *left_up, Point *right_bottom);
bool is_out_of_bounds(int w, int h, Point left_up, Point right_bottom);
void normalise_coordinates(int w, int h, Point *left_up, Point *right_bottom);