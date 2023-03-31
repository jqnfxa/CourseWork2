#include "../../interfaces/Utils/rectangle.h"
#include "../../interfaces/CLI/types_parser.h"
#include "../../interfaces/Geometry/matrix.h"

// TODO create tests
// TODO may be make argument for fill optional and fill with default color?
// TODO let points be out of range?
void draw_rectangle(Matrix *matrix, RectangleRequest *info)
{
	if(matrix == NULL || info == NULL)
	{
		return;
	}
	if(info->left_up.x >= matrix->width || info->left_up.y >= matrix->height)
	{
		return;
	}
	if(info->right_bottom.x >= matrix->width)
	{
		info->right_bottom.x = (int)matrix->width;
	}
	if(info->right_bottom.y >= matrix->height)
	{
		info->right_bottom.y = (int)matrix->height;
	}

	int width_y = min(info->width, (info->right_bottom.y - info->left_up.y) / 2 + 1);
	for(int width = 0; width < width_y; ++width)
	{
		draw_horizontal_line(matrix, info->left_up.x, info->right_bottom.x, info->left_up.y + width, info->color);
		draw_horizontal_line(matrix, info->left_up.x, info->right_bottom.x, info->right_bottom.y - width, info->color);
	}

	int width_x = min(info->width, (info->right_bottom.x - info->left_up.x) / 2 + 1);
	for(int width = 0; width < width_x; ++width)
	{
		draw_vertical_line(matrix, info->left_up.y, info->right_bottom.y, info->left_up.x + width, info->color);
		draw_vertical_line(matrix, info->left_up.y, info->right_bottom.y, info->right_bottom.x - width, info->color);
	}

	if(!match_flags(info->check_sum, FILL))
	{
		return;
	}

	Point left_up = {info->left_up.x + width_x, info->left_up.y + width_y};
	Point right_bottom = {info->right_bottom.x - width_x, info->right_bottom.y - width_y};

	if(!validate_area(&left_up, &right_bottom))
	{
		return;
	}

	Point pixel;
	for(pixel.y = left_up.y; pixel.y <= right_bottom.y; ++pixel.y)
	{
		for(pixel.x = left_up.x; pixel.x <= right_bottom.x; ++pixel.x)
		{
			set_pixel(matrix, pixel, info->fill_color);
		}
	}
}

void draw_vertical_line(Matrix *matrix, int y_1, int y_2, int x, int color)
{
	if(y_2 < y_1)
	{
		return;
	}

	Point pixel = {.x = x, .y = y_1};
	for(; pixel.y < matrix->height && pixel.y <= y_2; ++pixel.y)
	{
		set_pixel(matrix, pixel, color);
	}
}

void draw_horizontal_line(Matrix *matrix, int x_1, int x_2, int y, int color)
{
	if(x_2 < x_1)
	{
		return;
	}

	Point pixel = {.x = x_1, .y = y};
	for(; pixel.x < matrix->width && pixel.x <= x_2; ++pixel.x)
	{
		set_pixel(matrix, pixel, color);
	}
}