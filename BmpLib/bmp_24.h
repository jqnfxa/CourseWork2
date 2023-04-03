#pragma once

#include <stdint.h>

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
	int32_t biSize;
	int32_t biWidth;
	int32_t biHeight;
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
