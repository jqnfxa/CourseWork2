#pragma once

#include "../Declarations/structures.h"
#include <stdbool.h>

typedef struct RectangleRequest {
	Point left_up;
	Point right_bottom;

	int width;
	int color;
	int fill_color;
	char new_file[256];

	int check_sum;
} RectangleRequest;

bool validate_rectangle(RectangleRequest *request);
bool parse_rectangle_request(int argc, char *argv[], char *file_name, RectangleRequest *request);
