#pragma once

#include <stdint.h>

enum CompressionMethod {
	BI_RGB,
	BI_RLE8,
	BI_RLE4,
	BI_BITFIELDS,
	BI_JPEG,
	BI_PNG,
	BI_ALPHABITFIELDS,
	BI_CMYK = 11,
	BI_CMYKRLE8,
	BI_CMYKRLE4
};

extern const char *CompressionStrings[14];

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

typedef struct tagBITMAPV4HEADER {
	int32_t biSize;
	int32_t biWidth;
	int32_t biHeight;
	int16_t biPlanes;
	int16_t biBitCount;
	int32_t biCompression;
	int32_t biSizeImage;
	int32_t biXPPM;
	int32_t biYPPM;
	int32_t biClrUsed;
	int32_t biClrImportant;
	int32_t biRedMask;
	int32_t biGreenMask;
	int32_t biBlueMask;
	int32_t biAlphaMask;
	int32_t biCSType;
	uint8_t unused[36];
	int32_t biGammaRed;
	int32_t biGammaGreen;
	int32_t biGammaBlue;
} BITMAPV4HEADER;

#pragma pack(pop)

#pragma pack(push, 1)

typedef struct tagBITMAPV5HEADER {
	int32_t biSize;
	int32_t biWidth;
	int32_t biHeight;
	int16_t biPlanes;
	int16_t biBitCount;
	int32_t biCompression;
	int32_t biSizeImage;
	int32_t biXPPM;
	int32_t biYPPM;
	int32_t biClrUsed;
	int32_t biClrImportant;
	int32_t biRedMask;
	int32_t biGreenMask;
	int32_t biBlueMask;
	int32_t biAlphaMask;
	int32_t biCSType;
	uint8_t unused[36];
	int32_t biGammaRed;
	int32_t biGammaGreen;
	int32_t biGammaBlue;
	int32_t biIntent;
	int32_t biProfileData;
	int32_t biProfileSize;
	int32_t biReserved;
} BITMAPV5HEADER;

#pragma pack(pop)

#pragma pack(push, 1)

typedef struct tagBITMAPCOREHEADER {
	int32_t biSize;
	int16_t biWidth;
	int16_t biHeight;
	int16_t biPlanes;
	int16_t biBitCount;
} BITMAPCOREHEADER;

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
	uint32_t biXPPM;
	uint32_t biYPPM;
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
