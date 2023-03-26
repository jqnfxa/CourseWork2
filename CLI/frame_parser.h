#pragma once

#include <stdbool.h>

typedef struct FrameRequest {
	int type;
	int color;
	int width;
	char new_file[256];

	int check_sum;
} FrameRequest;

bool validate_frame(FrameRequest *request);
bool parse_frame_request(int argc, char *argv[], char *file_name, FrameRequest *request);