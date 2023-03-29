#pragma once

#include "../Shared/structures.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

;
#pragma pack(push, 1)

typedef struct tagBITMAPFILEHEADER {
	uint16_t bfType;
	uint32_t bfSize;
	uint16_t bfReserved1;
	uint16_t bfReserved2;
	uint32_t bfOffBits;
} BITMAPFILEHEADER;

#pragma pack(pop)

#pragma pack(push, 1)

typedef struct tagBITMAPINFOHEADER {
	uint32_t biSize;
	uint32_t biWidth;
	uint32_t biHeight;
	uint16_t biPlanes;
	uint16_t biBitCount;
	uint32_t biCompression;
	uint32_t biSizeImage;
	uint32_t biXPelsPerMeter;
	uint32_t biYPelsPerMeter;
	uint32_t biClrUsed;
	uint32_t biClrImportant;
} BITMAPINFOHEADER;

#pragma pack(pop)

#pragma pack(push, 1)

typedef struct tagRGB {
	uint8_t b;
	uint8_t g;
	uint8_t r;
} RGB;

#pragma pack(pop)

#pragma pack(push, 1)

typedef struct tagBMP {
	BITMAPFILEHEADER header;
	BITMAPINFOHEADER dib_header;
	Matrix matrix;
	uint32_t junk_bytes; // value to store junk bytes to alignment
} BMP;

#pragma pack(pop)

BMP *load_image(const char *filename);
bool read_pixel_matrix(FILE *file, Matrix *matrix, uint32_t alignment);
bool unload_image(const char *filename, BMP *picture);

void free_matrix(Matrix *matrix);
void free_bmp(BMP *ptr);
void safe_free_bmp(BMP **ptr);

bool safe_fread(void *dst, size_t size, size_t member_size, FILE *stream);
int rgb_to_int(RGB *color);
RGB int_to_rgb(int color);