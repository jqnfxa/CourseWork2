#pragma once

#include "query_structures.h"
#include <getopt.h>
#include <stdbool.h>

enum OPERATION_TYPE parse_user_command(int32_t argc, char *argv[]);
bool parse_rectangle_query(int32_t argc, char *argv[], char *file_name, RectangleQuery *query);
bool parse_circle_query(int32_t argc, char *argv[], char *file_name, CircleQuery *query);
bool parse_frame_query(int32_t argc, char *argv[], char *file_name, FrameQuery *query);
bool parse_rotate_query(int32_t argc, char *argv[], char *file_name, RotateQuery *query);