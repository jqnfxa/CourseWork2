#pragma once

#include <stdbool.h>
#include "../Shared/structures.h"

typedef struct RotateRequest
{
	Point left_up;
	Point right_bottom;

	enum ROTATE_DIRECTION direction;
	int angle;
	char new_file[256];

	int check_sum;
}
RotateRequest;

bool validate_rotate(RotateRequest *request);
bool parse_rotate_request(int argc, char *argv[], char *file_name, RotateRequest *request);