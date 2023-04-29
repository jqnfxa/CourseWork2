#include "image.h"
#include "../ExceptionHandler/logger.h"
#include "../Validator/validator.h"
#include "../BmpLib/bmp_parser.h"
#include <stddef.h>

Matrix crop(Matrix *matrix, Area *area)
{
	if(!is_valid_matrix(matrix) || !validate_area(area) || !validate_image_area(matrix->width, matrix->height, area))
	{
		return (Matrix){};
	}

	shrink_to_fit(matrix->width, matrix->height, area);

	Matrix sub = create(area->right_bottom.y - area->left_up.y, area->right_bottom.x - area->left_up.x);

	if(sub.grid == NULL)
	{
		log_error(OUT_OF_MEMORY, "create crop");
		return sub;
	}

	for(int32_t i = area->left_up.y, row = 0; i < area->right_bottom.y; ++i, ++row)
	{
		for(int32_t j = area->left_up.x, column = 0; j < area->right_bottom.x; ++j, ++column)
		{
			sub.grid[row][column] = matrix->grid[i][j];
		}
	}
	return sub;
}

void create_canvas(Matrix *matrix, CanvasQuery *query)
{
	if(query == NULL)
	{
		return;
	}
	BMP *image = create_image(query->width,query->height, query->color);
	unload_image(query->new_file, image);
	safe_free_bmp(&image);
}

void paste(Matrix *dst, Matrix *src, Point *left_up)
{
	if(dst == NULL || src == NULL || left_up == NULL ||
	   left_up->x >= dst->width || left_up->y >= dst->height ||
	   left_up->y + src->height < 0 || left_up->x + src->width < 0)
	{
		return;
	}

	for(int32_t i = 0, y = left_up->y; i < src->height; ++i, ++y)
	{
		for(int32_t j = 0, x = left_up->x; j < src->width; ++j, ++x)
		{
			set_pixel(dst, x, y, src->grid[i][j]);
		}
	}
}

void paste_if(Matrix *dst, Matrix *src, Point *left_up, int32_t skip_color)
{
	if(dst == NULL || src == NULL || left_up == NULL ||
	   left_up->x >= dst->width || left_up->y >= dst->height ||
	   left_up->y + src->height < 0 || left_up->x + src->width < 0)
	{
		return;
	}

	for(int32_t i = 0, y = left_up->y; i < src->height; ++i, ++y)
	{
		for(int32_t j = 0, x = left_up->x; j < src->width; ++j, ++x)
		{
			if(src->grid[i][j] != skip_color)
			{
				set_pixel(dst, x, y, src->grid[i][j]);
			}
		}
	}
}

void set_pixel(Matrix *matrix, int32_t x, int32_t y, int32_t color)
{
	Point point = {x, y};
	if(!is_in_bound(matrix, &point))
	{
		return;
	}

	matrix->grid[y][x] = color;
}

void set_pixel_avoid(Matrix *matrix, int32_t x, int32_t y, int32_t color, int danger_color)
{
	Point point = {x, y};
	if(!is_in_bound(matrix, &point))
	{
		return;
	}

	if(matrix->grid[y][x] != danger_color)
	{
		matrix->grid[y][x] = color;
	}
}

int32_t rgb_to_int(RGB *color)
{
	if(color == NULL)
	{
		return 0;
	}
	return (color->r << 16) | (color->g << 8) | (color->b);
}

int32_t invert_color(int32_t color)
{
	int new_color = 0;

	new_color |= (255 - (color >> 16 & 0xff)) << 16;
	new_color |= (255 - (color >> 8 & 0xff)) << 8;
	new_color |= (255 - (color & 0xff));
	return new_color;
}


RGB int_to_rgb(int32_t color)
{
	return (RGB){.r = color >> 16 & 0xff, .g = color >> 8 & 0xff, .b = color & 0xff};
}