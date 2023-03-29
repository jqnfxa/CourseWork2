#include "test_BmpLib.h"
#include "../BmpLib/bmp_parser.h"
#include <stdlib.h>

void test_bmp_read()
{
	{
		BMP *a = load_image("../Tests/image.bmp");

		if(a == NULL || a->header.bfSize != 374 || a->junk_bytes != 2 || a->dib_header.biWidth != 10 || a->dib_header.biHeight != 10)
		{
			fprintf(stderr, "failed #1");
			safe_free_bmp(&a);
			return;
		}
		safe_free_bmp(&a);
	}
	{
		BMP *a = load_image("../Tests/screen.bmp");

		if(a == NULL || a->header.bfSize != 1580766 || a->dib_header.biWidth != 905 || a->dib_header.biHeight != 582)
		{
			fprintf(stderr, "failed #2");
			safe_free_bmp(&a);
			return;
		}
		safe_free_bmp(&a);
	}
	fprintf(stderr, "passed tmp create\n");
}

void test_bmp_write()
{
	BMP picture;

	// header
	picture.header.bfType = 0x4d42;
	picture.header.bfSize = 102;
	picture.header.bfReserved1 = 0;
	picture.header.bfReserved1 = 0;
	picture.header.bfOffBits = 54;

	// dib
	picture.dib_header.biSize = 40;
	picture.dib_header.biWidth = 4;
	picture.dib_header.biHeight = 4;
	picture.dib_header.biPlanes = 1;
	picture.dib_header.biBitCount = 24;
	picture.dib_header.biCompression = 0;
	picture.dib_header.biSizeImage = picture.header.bfSize - 54;
	picture.dib_header.biXPelsPerMeter = 3780;
	picture.dib_header.biYPelsPerMeter = 3780;
	picture.dib_header.biClrUsed = 0;
	picture.dib_header.biClrImportant = 0;

	// matrix
	picture.matrix.width = picture.dib_header.biWidth;
	picture.matrix.height = picture.dib_header.biHeight;
	picture.matrix.grid = (int32_t **)calloc(picture.matrix.height, sizeof(int32_t *));

	for(size_t i = 0; i < picture.matrix.height; ++i)
	{
		picture.matrix.grid[i] = (int32_t *)calloc(picture.matrix.width, sizeof(int32_t));
	}

	picture.matrix.grid[0][0] = 0xff0000;

	// alignment
	picture.junk_bytes = 0;

	unload_image("test.bmp", &picture);

	free_bmp(&picture);
}