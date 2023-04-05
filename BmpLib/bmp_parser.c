#include "bmp_parser.h"
#include "../Draw/image.h"
#include "../ExceptionHandler/logger.h"
#include <stdlib.h>

BMP *load_image(const char *filename)
{
	// open file to read
	FILE *file = fopen(filename, "rb");

	// failed to open file
	if(file == NULL)
	{
		log_error(FILE_OPEN, filename);
		return NULL;
	}

	BMP *bmp = (BMP *)calloc(1, sizeof(BMP));

	// read BITMAPFILEHEADER
	if(!safe_fread((void *)&bmp->header, sizeof(bmp->header), 1, file))
	{
		log_error(FILE_READ, filename);
		goto fail;
	}

	// check if file prefix is BM
	if(bmp->header.bfType != BMP_INDENTIFIER)
	{
		log_error(FILE_INVALID, filename);
		goto fail;
	}

	// read file info header
	if(!safe_fread((void *)&bmp->dib_header, sizeof(bmp->dib_header), 1, file))
	{
		log_error(FILE_READ, filename);
		goto fail;
	}

	// check the bitmap info. If it is not 24-bit, and uncompressed return false
	if(bmp->dib_header.biSize != 40 || bmp->dib_header.biBitCount != 24 ||
	   bmp->dib_header.biCompression != 0 ||
	   (bmp->dib_header.biClrUsed != 0 && bmp->dib_header.biClrImportant != 0))
	{
		log_error(UNSUPPORTED_TYPE, filename);
		goto fail;
	}

	// set new file pointer position at start of pixels array
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
	read_pixel_matrix(file,bmp->dib_header.biWidth,bmp->dib_header.biHeight, &bmp->matrix,bmp->junk_bytes);

	fclose(file);
	return bmp;
fail:
	free(bmp);
	fclose(file);
	return NULL;
}

BMP *create_image(int width, int height, int color)
{
	BMP *image = malloc(sizeof(BMP));

	if(image == NULL)
	{
		log_error(OUT_OF_MEMORY, "create_image");
		return image;
	}

	int closest;
	for(closest = width * 3; closest % 4 != 0; ++closest) {}

	image->junk_bytes = closest - width * 3;
	image->header.bfType = BMP_INDENTIFIER;
	image->header.bfSize = image->junk_bytes * height + 3 * width * height + 54;
	image->header.bfReserved1 = 0;
	image->header.bfReserved2 = 0;
	image->header.bfOffBits = 54;

	image->dib_header.biSize = 40;
	image->dib_header.biWidth = width;
	image->dib_header.biHeight = height;
	image->dib_header.biPlanes = 1;
	image->dib_header.biBitCount = 24;
	image->dib_header.biCompression = 0;
	image->dib_header.biSizeImage = image->header.bfSize - image->header.bfOffBits;
	image->dib_header.biXPelsPerMeter = 2835;
	image->dib_header.biYPelsPerMeter = 2835;
	image->dib_header.biClrUsed = 0;
	image->dib_header.biClrImportant = 0;

	image->matrix = create(height, width);

	Point point;
	for(int i = 0; i < image->matrix.height; ++i)
	{
		for(int j = 0; j < image->matrix.width; ++j)
		{
			point.x = i;
			point.y = j;
			set_pixel(&image->matrix, &point, color);
		}
	}

	return image;
}

bool read_pixel_matrix(FILE *file, int32_t width, int32_t height, Matrix *matrix, uint32_t alignment)
{
	if(file == NULL || matrix == NULL)
	{
		return false;
	}

	*matrix = create(height, width);

	if(matrix->grid == NULL)
	{
		log_error(OUT_OF_MEMORY, "read_pixel_matrix");
		return false;
	}

	RGB pixel;

	// we need to reed scan lines
	for(int32_t i = matrix->height - 1; i >= 0; i--)
	{
		for(int32_t j = 0; j < matrix->width; ++j)
		{
			if(!feof(file) && safe_fread((void *)&pixel, sizeof(RGB), 1, file))
			{
				matrix->grid[i][j] = rgb_to_int(&pixel);
			}
			else
			{
				log_error(FILE_INVALID, NULL);
				destroy(matrix);
				return false;
			}
		}

		// When scan line is completed we need to skip the junk bytes
		fseek(file, alignment, SEEK_CUR);
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
				log_error(FILE_WRITE, NULL);
				return false;
			}
		}

		// add junk bytes if required
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
		log_error(FILE_OPEN, filename);
		return false;
	}

	if(!fwrite(&picture->header, sizeof(BITMAPFILEHEADER), 1, output))
	{
		log_error(FILE_WRITE, "header");
		goto fail;
	}
	if(!fwrite(&picture->dib_header, sizeof(BITMAPINFOHEADER), 1, output))
	{
		log_error(FILE_WRITE, "dib header");
		goto fail;
	}
	if(!write_pixel_matrix(output, &picture->matrix, picture->junk_bytes))
	{
		log_error(FILE_WRITE, "pixel matrix");
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