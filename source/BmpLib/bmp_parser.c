#include "../../interfaces/BMPLib/bmp_parser.h"
#include "../../interfaces/Geometry/matrix.h"
#include <stdlib.h>

// TODO more accurate parser
BMP *load_image(const char *filename)
{
	// open file to read
	FILE *file = fopen(filename, "rb");

	// failed to open file
	if(file == NULL)
	{
		return NULL;
	}

	BMP *bmp = (BMP *)calloc(1, sizeof(BMP));

	// read BITMAPFILEHEADER
	if(!safe_fread((void *)&bmp->header, sizeof(bmp->header), 1, file))
	{
		goto fail;
	}

	// check if file prefix is BM
	if(bmp->header.bfType != BMP_INDENTIFIER)
	{
		goto fail;
	}

	// read file info header
	if(!safe_fread((void *)&bmp->dib_header, sizeof(bmp->dib_header), 1, file))
	{
		goto fail;
	}

	// check the bitmap info. If it is not 24-bit, and uncompressed,
	// and the colors used and are important is not 0, return false.
	if(bmp->dib_header.biSize != 40 || bmp->dib_header.biBitCount != 24 ||
	   bmp->dib_header.biCompression != 0 ||
	   (bmp->dib_header.biClrUsed != 0 && bmp->dib_header.biClrImportant != 0))
	{
		goto fail;
	}

	// set new file pointer position at 54 from the beginning of
	// the file. 0 to 53 are used to store BITMAPFILEINFO and
	// BITMAPINFOHEADER.
	fseek(file, bmp->header.bfOffBits, SEEK_SET);

	// manually calculate size if required
	if(bmp->dib_header.biSizeImage == 0)
	{
		bmp->dib_header.biSizeImage = bmp->header.bfSize - bmp->header.bfOffBits;
	}

	// calculate memory alignment
	uint32_t alignment = (bmp->dib_header.biSizeImage - bmp->dib_header.biWidth * bmp->dib_header.biHeight * 3);
	bmp->junk_bytes = (alignment == 0 ? 0 : alignment / bmp->dib_header.biHeight);

	// read pixel matrix
	bmp->matrix.width = bmp->dib_header.biWidth;
	bmp->matrix.height = bmp->dib_header.biHeight;
	read_pixel_matrix(file, &bmp->matrix, bmp->junk_bytes);

	fclose(file);
	return bmp;
fail:
	free(bmp);
	fclose(file);
	return NULL;
}

bool read_pixel_matrix(FILE *file, Matrix *matrix, uint32_t alignment)
{
	if(file == NULL || matrix == NULL)
	{
		return false;
	}

	*matrix = create(matrix->height, matrix->width);

	RGB pixel;

	// we need to reed scan lines
	for(int i = (int)matrix->height - 1; i >= 0; i--)
	{
		for(int j = 0; j < matrix->width; ++j)
		{
			if(!feof(file) && safe_fread((void *)&pixel, sizeof(RGB), 1, file))
			{
				matrix->grid[i][j] = rgb_to_int(&pixel);
			}
			else
			{
				destroy(matrix);
				return false;
			}
		}

		// When one scan line is completed
		// We need to skip the junk bytes
		if(alignment != 0)
		{
			fseek(file, alignment, SEEK_CUR);
		}
	}

	return true;
}

bool write_pixel_matrix(FILE *file, Matrix *matrix, uint32_t alignment)
{
	if(file == NULL || matrix == NULL)
	{
		return false;
	}

	RGB pixel;
	char empty_byte = 0;

	// we need to write scan lines
	for(int i = (int)matrix->height - 1; i >= 0; i--)
	{
		for(int j = 0; j < matrix->width; ++j)
		{
			pixel = int_to_rgb(matrix->grid[i][j]);
			if(!fwrite(&pixel, sizeof(RGB), 1, file))
			{
				return false;
			}
		}

		// When one scan line is completed
		// We need to add junk bytes
		if(alignment != 0 && !fwrite(&empty_byte, sizeof(char), alignment, file))
		{
			return false;
		}
	}

	return true;
}

bool unload_image(const char *filename, BMP *picture)
{
	if(picture == NULL)
	{
		return false;
	}

	FILE *output = fopen(filename, "wb");

	if(output == NULL)
	{
		return false;
	}

	if(!fwrite(&picture->header, sizeof(BITMAPFILEHEADER), 1, output))
	{
		goto fail;
	}
	if(!fwrite(&picture->dib_header, sizeof(BITMAPINFOHEADER), 1, output))
	{
		goto fail;
	}
	if(!write_pixel_matrix(output, &picture->matrix, picture->junk_bytes))
	{
		goto fail;
	}

	fclose(output);
	return true;
fail:
	fclose(output);
	return false;
}

void free_bmp(BMP *ptr)
{
	if(ptr == NULL)
	{
		return;
	}
	destroy(&ptr->matrix);
}

void safe_free_bmp(BMP **ptr)
{
	if(ptr == NULL)
	{
		return;
	}
	free_bmp(*ptr);
	free(*ptr);
	*ptr = NULL;
}

bool safe_fread(void *dst, size_t size, size_t member_size, FILE *stream)
{
	if(dst == NULL || stream == NULL)
	{
		return false;
	}

	// check if we have read exactly valid count of bytes
	return fread(dst, size, member_size, stream) == member_size;
}

int rgb_to_int(RGB *color)
{
	if(color == NULL)
	{
		return 0;
	}
	return (color->r << 16) | (color->g << 8) | (color->b);
}

RGB int_to_rgb(int color)
{
	return (RGB){.r = color >> 16 & 0xff, .g = color >> 8 & 0xff, .b = color & 0xff};
}