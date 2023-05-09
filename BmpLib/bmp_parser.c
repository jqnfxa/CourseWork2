#include "bmp_parser.h"
#include "../Draw/image.h"
#include "../ExceptionHandler/logger.h"
#include "../Validator/validator.h"
#include <stdlib.h>

const char *CompressionStrings[14] = {
	"BI_RGB",
	"BI_RLE8",
	"BI_RLE4",
	"BI_BITFIELDS",
	"BI_JPEG",
	"BI_PNG",
	"BI_ALPHABITFIELDS",
	"UNKNOWN",
	"UNKNOWN",
	"UNKNOWN",
	"UNKNOWN",
	"BI_CMYK",
	"BI_CMYKRLE8",
	"BI_CMYKRLE4"
};

BMP *load_image(const char *filename)
{
	FILE *file = fopen(filename, "rb");

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

	// check if file prefix is 'BM'
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

	// check the bitmap info
	if(bmp->dib_header.biSize != 40 ||
	   bmp->dib_header.biBitCount != 24 ||
	   bmp->dib_header.biCompression != 0 ||
	   bmp->dib_header.biPlanes != 1)
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
	read_pixel_matrix(file, bmp->dib_header.biWidth, bmp->dib_header.biHeight, &bmp->matrix, bmp->junk_bytes);

	fclose(file);
	return bmp;
fail:
	free(bmp);
	fclose(file);
	return NULL;
}

BMP *create_image(int32_t width, int32_t height, int32_t color)
{
	BMP *image = malloc(sizeof(BMP));

	if(image == NULL)
	{
		log_error(OUT_OF_MEMORY, "create_image");
		return image;
	}

	int32_t closest;
	for(closest = width * 3; closest % 4 != 0; ++closest) {}

	image->junk_bytes = closest - width * 3;
	image->header.bfType = BMP_INDENTIFIER;
	image->header.bfSize = image->junk_bytes * height + 3u * width * height + 54;
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
	image->dib_header.biXPPM = 2835;
	image->dib_header.biYPPM = 2835;
	image->dib_header.biClrUsed = 0;
	image->dib_header.biClrImportant = 0;

	image->matrix = create_filled(height, width, color);

	return image;
}

void dump_info(FILE *stream, const char *filename)
{
	FILE *file = fopen(filename, "rb");

	if(file == NULL || !is_valid_bmp_name(filename))
	{
		log_error(FILE_OPEN, filename);
		return;
	}

	if(!dump_info_header(stream, file))
	{
		fclose(file);
		return;
	}
	if(!dump_dib_header(stream, file))
	{
		fclose(file);
		log_error(FILE_INVALID, "");
		return;
	}
	fclose(file);
}

bool dump_info_header(FILE *stream, FILE *file)
{
	if(stream == NULL || file == NULL)
	{
		return false;
	}

	BITMAPFILEHEADER file_header;

	if(!safe_fread((void *)&file_header, sizeof(file_header), 1, file))
	{
		return false;
	}

	if(file_header.bfType != BMP_INDENTIFIER)
	{
		fprintf(stream, "File is not bmp\n");
		return false;
	}

	fprintf(stream, "Bitmap info header\n");
	fprintf(stream, "bfType:      0x%x\n", file_header.bfType);
	fprintf(stream, "bfSize:      %u\n", file_header.bfSize);
	fprintf(stream, "bfReserved1: %d\n", file_header.bfReserved1);
	fprintf(stream, "bfReserved2: %d\n", file_header.bfReserved2);
	fprintf(stream, "bfOffBits:   %u\n\n", file_header.bfOffBits);

	return true;
}

bool dump_dib_header(FILE *stream, FILE *file)
{
	if(stream == NULL || file == NULL)
	{
		return false;
	}

	int dib_header_size = 0;

	if(!safe_fread((void *)&dib_header_size, sizeof(int), 1, file))
	{
		return false;
	}

	fseek(file, 14, SEEK_SET);

	switch(dib_header_size)
	{
		case 12:
		{
			BITMAPCOREHEADER dib_header;

			if(!safe_fread((void *)&dib_header, sizeof(dib_header), 1, file))
			{
				return false;
			}

			fprintf(stream, "\nBitmap DIB header (BITMAPCOREHEADER)\n");
			fprintf(stream, "biSize:      %u\n", dib_header.biSize);
			fprintf(stream, "biWidth:     %u\n", dib_header.biWidth);
			fprintf(stream, "biHeight:    %u\n", dib_header.biHeight);
			fprintf(stream, "biPlanes:    %d\n", dib_header.biPlanes);
			fprintf(stream, "biBitCount:  %d\n", dib_header.biBitCount);
		}
		break;
		case 40:
		{
			BITMAPINFOHEADER dib_header;

			if(!safe_fread((void *)&dib_header, sizeof(dib_header), 1, file))
			{
				return false;
			}

			fprintf(stream, "\nBitmap DIB header (BITMAPINFOHEADER)\n");
			fprintf(stream, "biSize:      %u\n", dib_header.biSize);
			fprintf(stream, "biWidth:     %u\n", dib_header.biWidth);
			fprintf(stream, "biHeight:    %u\n", dib_header.biHeight);
			fprintf(stream, "biPlanes:    %d\n", dib_header.biPlanes);
			fprintf(stream, "biBitCount:  %d\n", dib_header.biBitCount);
			fprintf(stream, "biCompress:  %s\n", CompressionStrings[dib_header.biCompression % 14]);
			fprintf(stream, "biSizeImage: %u\n", dib_header.biSizeImage);
			fprintf(stream, "biXPPM:      %d\n", dib_header.biXPPM);
			fprintf(stream, "biYPPM:      %d\n", dib_header.biYPPM);
			fprintf(stream, "biClrUsed:   %d\n", dib_header.biClrUsed);
			fprintf(stream, "biImportant: %d\n", dib_header.biClrImportant);
		}
		break;
		case 108:
		{
			BITMAPV4HEADER dib_header;

			if(!safe_fread((void *)&dib_header, sizeof(dib_header), 1, file))
			{
				return false;
			}

			fprintf(stream, "\nBitmap DIB header (BITMAPV4HEADER)\n");
			fprintf(stream, "biSize:      %u\n", dib_header.biSize);
			fprintf(stream, "biWidth:     %u\n", dib_header.biWidth);
			fprintf(stream, "biHeight:    %u\n", dib_header.biHeight);
			fprintf(stream, "biPlanes:    %d\n", dib_header.biPlanes);
			fprintf(stream, "biBitCount:  %d\n", dib_header.biBitCount);
			fprintf(stream, "biCompress:  %s\n", CompressionStrings[dib_header.biCompression % 14]);
			fprintf(stream, "biSizeImage: %u\n", dib_header.biSizeImage);
			fprintf(stream, "biXPPM:      %d\n", dib_header.biXPPM);
			fprintf(stream, "biYPPM:      %d\n", dib_header.biYPPM);
			fprintf(stream, "biClrUsed:   %d\n", dib_header.biClrUsed);
			fprintf(stream, "biImportant: %d\n", dib_header.biClrImportant);
			fprintf(stream, "biRedMask:   0x%x\n", dib_header.biRedMask);
			fprintf(stream, "biGreenMask: 0x%x\n", dib_header.biGreenMask);
			fprintf(stream, "biBlueMask:  0x%x\n", dib_header.biBlueMask);
			fprintf(stream, "biAlphaMask: 0x%x\n", dib_header.biAlphaMask);
			fprintf(stream, "biCSType:    0x%x\n", dib_header.biCSType);
			fprintf(stream, "biGammaRed:  0x%x\n", dib_header.biGammaRed);
			fprintf(stream, "biGammaGrn:  0x%x\n", dib_header.biGammaGreen);
			fprintf(stream, "biGammaBlue: 0x%x\n", dib_header.biGammaBlue);
		}
		break;
		case 124:
		{
			BITMAPV5HEADER dib_header;

			if(!safe_fread((void *)&dib_header, sizeof(dib_header), 1, file))
			{
				return false;
			}

			fprintf(stream, "\nBitmap DIB header (BITMAPV5HEADER)\n");
			fprintf(stream, "biSize:      %u\n", dib_header.biSize);
			fprintf(stream, "biWidth:     %u\n", dib_header.biWidth);
			fprintf(stream, "biHeight:    %u\n", dib_header.biHeight);
			fprintf(stream, "biPlanes:    %d\n", dib_header.biPlanes);
			fprintf(stream, "biBitCount:  %d\n", dib_header.biBitCount);
			fprintf(stream, "biCompress:  %s\n", CompressionStrings[dib_header.biCompression % 14]);
			fprintf(stream, "biSizeImage: %u\n", dib_header.biSizeImage);
			fprintf(stream, "biXPPM:      %d\n", dib_header.biXPPM);
			fprintf(stream, "biYPPM:      %d\n", dib_header.biYPPM);
			fprintf(stream, "biClrUsed:   %d\n", dib_header.biClrUsed);
			fprintf(stream, "biImportant: %d\n", dib_header.biClrImportant);
			fprintf(stream, "biRedMask:   0x%x\n", dib_header.biRedMask);
			fprintf(stream, "biGreenMask: 0x%x\n", dib_header.biGreenMask);
			fprintf(stream, "biBlueMask:  0x%x\n", dib_header.biBlueMask);
			fprintf(stream, "biAlphaMask: 0x%x\n", dib_header.biAlphaMask);
			fprintf(stream, "biCSType:    0x%x\n", dib_header.biCSType);
			fprintf(stream, "biGammaRed:  0x%x\n", dib_header.biGammaRed);
			fprintf(stream, "biGammaGrn:  0x%x\n", dib_header.biGammaGreen);
			fprintf(stream, "biGammaBlue: 0x%x\n", dib_header.biGammaBlue);
			fprintf(stream, "biIntent:    0x%x\n", dib_header.biIntent);
			fprintf(stream, "biProfData:  0x%x\n", dib_header.biProfileData);
			fprintf(stream, "biProfSize:  0x%x\n", dib_header.biProfileSize);
			fprintf(stream, "biReserved:  0x%x\n", dib_header.biReserved);
		}
		break;
		default:
			return false;
	}

	return true;
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

		// skip the junk bytes
		fseek(file, alignment, SEEK_CUR);
	}

	return true;
}

bool write_pixel_matrix(FILE *file, Matrix *matrix, uint32_t alignment)
{
	if(file == NULL || !is_valid_matrix(matrix))
	{
		return false;
	}

	RGB pixel;
	char empty_byte = 0;

	// we need to write scan lines
	for(int32_t i = matrix->height - 1; i >= 0; --i)
	{
		for(int32_t j = 0; j < matrix->width; ++j)
		{
			pixel = int_to_rgb(matrix->grid[i][j]);
			if(!fwrite(&pixel, sizeof(RGB), 1, file))
			{
				log_error(FILE_WRITE, NULL);
				return false;
			}
		}

		// add junk bytes if required
		if(alignment != 0 && fwrite(&empty_byte, sizeof(char), alignment, file) != alignment)
		{
			return false;
		}
	}

	return true;
}

void resize_image(BMP *image)
{
	if(image == NULL || image->matrix.grid == NULL)
	{
		return;
	}

	int32_t closest;
	for(closest = image->matrix.width * 3; closest % 4 != 0; ++closest) {}

	image->junk_bytes = closest - image->matrix.width * 3;
	image->header.bfSize = image->junk_bytes * image->matrix.height + sizeof(RGB) * image->matrix.width * image->matrix.height + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	image->dib_header.biWidth = image->matrix.width;
	image->dib_header.biHeight = image->matrix.height;
	image->dib_header.biSizeImage = image->junk_bytes * image->matrix.height + sizeof(RGB) * image->matrix.width * image->matrix.height;
}

bool unload_image(const char *filename, BMP *picture)
{
	if(picture == NULL || filename == NULL || *filename == '\0')
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