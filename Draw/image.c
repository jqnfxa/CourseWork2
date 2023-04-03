#include "image.h"
#include "../Validator/validator.h"
#include <stddef.h>

Matrix crop(Matrix *matrix, Area *area)
{
	if(matrix == NULL || !validate_area(area) || !validate_image_area(matrix->width, matrix->height, area))
	{
		return (Matrix){};
	}

	shrink_to_fit(matrix->width, matrix->height, area);

	Matrix sub = create(area->right_bottom.y - area->left_up.y, area->right_bottom.x - area->left_up.x);

	if(sub.grid == NULL)
	{
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

void paste(Matrix *dst, Matrix *src, Point *left_up)
{
	if(dst == NULL || src == NULL || left_up == NULL || left_up->x >= dst->width || left_up->y >= dst->height || left_up->y + src->height < 0 || left_up->x + src->width < 0)
	{
		return;
	}

	Point pixel = {.x = left_up->x, .y = left_up->y};

	for(int32_t i = 0; i < src->height; ++i, ++pixel.y)
	{
		pixel.x = left_up->x;
		for(int32_t j = 0; j < src->width; ++j, ++pixel.x)
		{
			set_pixel(dst, &pixel, src->grid[i][j]);
		}
	}
}

void set_pixel(Matrix *matrix, Point *point, int32_t color)
{
	if(!is_in_bound(matrix, point))
	{
		return;
	}

	matrix->grid[point->y][point->x] = color;
}

int32_t rgb_to_int(RGB *color)
{
	if(color == NULL)
	{
		return 0;
	}
	return (color->r << 16) | (color->g << 8) | (color->b);
}

RGB int_to_rgb(int32_t color)
{
	return (RGB){.r = color >> 16 & 0xff, .g = color >> 8 & 0xff, .b = color & 0xff};
}