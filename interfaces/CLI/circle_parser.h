#pragma once

#include "../Declarations/structures.h"
#include <stdbool.h>

typedef struct CircleRequest {
	Point left_up;
	Point right_bottom;

	Point center;
	int radius;

	int width;
	int color;
	int fill_color;
	char new_file[256];

	int check_sum;
} CircleRequest;

bool validate_circle(CircleRequest *request);
int get_circle_type(CircleRequest *request);
bool parse_circle_request(int argc, char *argv[], char *file_name, CircleRequest *request);