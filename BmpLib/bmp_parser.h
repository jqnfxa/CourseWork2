#pragma once

#include "../Geometry/matrix.h"
#include "bmp_structures.h"
#include <stdio.h>

#define BMP_INDENTIFIER 0x4d42

typedef struct tagBMP {
	BITMAPFILEHEADER header;
	BITMAPINFOHEADER dib_header;
	Matrix matrix;
	uint32_t junk_bytes; // value to store junk bytes to alignment
} BMP;

// I/O
BMP *load_image(const char *filename);
BMP *create_image(int32_t width, int32_t height, int32_t color);
void dump_info(FILE *stream, const char *filename);
bool dump_info_header(FILE *stream, FILE *file);
bool dump_dib_header(FILE *stream, FILE *file);
bool read_pixel_matrix(FILE *file, int32_t width, int32_t height, Matrix *matrix, uint32_t alignment);
void resize_image(BMP *image);
bool unload_image(const char *filename, BMP *picture);

void free_bmp(BMP *ptr);
void safe_free_bmp(BMP **ptr);

bool safe_fread(void *dst, size_t size, size_t member_size, FILE *stream);
