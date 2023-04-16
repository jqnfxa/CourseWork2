#pragma once

#include "../Geometry/structures.h"
#include <stdbool.h>

bool parse_point_values(const char *argument, Point *point, const char *option_name);
bool parse_int(const char *argument, int *val, const char *option_name, int base);
bool parse_color(const char *argument, int *color, const char *option_name);
bool parse_file_name(const char *argument, char *name, const char *option_name);
bool parse_points(const char *argument, int32_t **arr, int32_t *count, const char *option_name);