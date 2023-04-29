#pragma once

#include "query_structures.h"
#include <getopt.h>
#include <stdbool.h>

extern bool (*QueryFunctions[7])(int32_t argc, char *argv[], char *file_name, void *query);
extern bool (*TaskFunctions[7])(Matrix *matrix, void *query);

void free_query(int32_t query_idx, void *query);
void *get_query_structure(int32_t idx);
void get_file_to_unload(int32_t query_idx, void *query, char *file_name);

enum OPERATION_TYPE parse_user_command(int32_t argc, char *argv[]);
bool parse_rectangle_query(int32_t argc, char *argv[], char *file_name, RectangleQuery *query);
bool parse_circle_query(int32_t argc, char *argv[], char *file_name, CircleQuery *query);
bool parse_frame_query(int32_t argc, char *argv[], char *file_name, FrameQuery *query);
bool parse_rotate_query(int32_t argc, char *argv[], char *file_name, RotateQuery *query);
bool parse_line_query(int32_t argc, char *argv[], char *file_name, LineQuery *query);
bool parse_polygon_query(int32_t argc, char *argv[], char *file_name, PolygonQuery *query);
bool parse_canvas_query(int32_t argc, char *argv[], char *file_name, CanvasQuery *query);