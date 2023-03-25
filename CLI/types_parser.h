#pragma once
#include <stdbool.h>
#include "../Shared/structures.h"

bool parse_point_values(const char *argument, Point *point, const char *option_name);
bool parse_int(const char *argument, int *val, const char *option_name, int base);
bool parse_color(const char *argument, int *color, const char *option_name);
bool parse_file_name(const char *argument, char *name, const char *option_name);
bool is_valid_bmp(const char *file_name);
bool is_valid_rgb(int color);
