#include "../../interfaces/Utils/rectangle.h"
#include "../../interfaces/CLI/types_parser.h"
#include "../../interfaces/Utils/matrix.h"

#define max(a, b) (a > b ? a : b)
#define min(a, b) (a < b ? a : b)

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

	for(int row = left_up.y; row <= right_bottom.y; ++row)
	{
		for(int column = left_up.x; column <= right_bottom.x; ++column)
		{
			set_pixel(matrix, (Point){column, row}, info->fill_color);
		}
	}
}

void draw_vertical_line(Matrix *matrix, int y_1, int y_2, int x, int color)
{
	if(y_2 < y_1)
	{
		return;
	}

	for(int row = y_1; row < matrix->height && row <= y_2; ++row)
	{
		set_pixel(matrix, (Point){x, row}, color);
	}
}

void draw_horizontal_line(Matrix *matrix, int x_1, int x_2, int y, int color)
{
	if(x_2 < x_1)
	{
		return;
	}

	for(int column = x_1; column < matrix->width && column <= x_2; ++column)
	{
		set_pixel(matrix, (Point){column, y}, color);
	}
}