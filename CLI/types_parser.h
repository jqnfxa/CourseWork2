#pragma once
#include <stdbool.h>
#include "../Shared/structures.h"

bool parse_point_values(Point *point, const char *option_name);
bool parse_int(int *val, const char *option_name, int base);
bool parse_color(int *color, const char *option_name);
bool parse_file_name(char *name, const char *option_name);
bool is_valid_bmp(const char *file_name);
bool is_valid_rgb(int color);
