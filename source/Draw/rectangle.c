#include "../../interfaces/Utils/rectangle.h"
#include "../../interfaces/CLI/types_parser.h"
#include "../../interfaces/Geometry/matrix.h"

void draw_rectangle(Matrix *matrix, RectangleRequest *info)
{
	if(matrix == NULL || info == NULL)
	{
		return;
	}
	// image will be out of bounds any way
	if(info->left_up.x >= matrix->width || info->left_up.y >= matrix->height ||
	   info->right_bottom.x < 0 || info->right_bottom.y < 0)
	{
		return;
	}

	Point diff = {info->right_bottom.x - info->left_up.x, info->right_bottom.y - info->left_up.y};

	// width is too big (no fill color possible)
	if(info->width >= max((diff.x + 1) / 2, (diff.y + 1) / 2))
	{
		draw_filled_rectangle(matrix, info->left_up, info->right_bottom, info->color);
		return;
	}

	// upper area
	draw_filled_rectangle(matrix,
						  info->left_up,
						  (Point){info->right_bottom.x, info->left_up.y + info->width},
						  info->color);

	// bottom area
	draw_filled_rectangle(matrix,
						  (Point){info->left_up.x, info->right_bottom.y - info->width},
						  info->right_bottom,
						  info->color);

	// left
	draw_filled_rectangle(matrix,
						  (Point){info->left_up.x, info->left_up.y + info->width},
						  (Point){info->left_up.x + info->width, info->right_bottom.y - info->width},
						  info->color);

	// left
	draw_filled_rectangle(matrix,
						  (Point){info->right_bottom.x - info->width, info->left_up.x + info->width},
						  (Point){info->right_bottom.x, info->right_bottom.y - info->width},
						  info->color);

	if(!match_flags(info->check_sum, FILL))
	{
		return;
	}

	int width_y = min(info->width, (info->right_bottom.y - info->left_up.y) / 2 + 1);
	int width_x = min(info->width, (info->right_bottom.x - info->left_up.x) / 2 + 1);

	Point left_up = {info->left_up.x + width_x, info->left_up.y + width_y};
	Point right_bottom = {info->right_bottom.x - width_x, info->right_bottom.y - width_y};

	draw_filled_rectangle(matrix, left_up, right_bottom, info->fill_color);
}

void draw_filled_rectangle(Matrix *matrix, Point left_up, Point right_bottom, int color)
{
	if(matrix == NULL || !validate_area(&left_up, &right_bottom))
	{
		return;
	}
	// check if we actually need to draw something
	if(is_out_of_bounds(matrix->width, matrix->height, left_up, right_bottom))
	{
		return;
	}

	normalise_coordinates(matrix->width, matrix->height, &left_up, &right_bottom);

	Point pixel = {left_up.x, left_up.y};

	for(pixel.y = left_up.y; pixel.y <= right_bottom.y; ++pixel.y)
	{
		for(pixel.x = left_up.x; pixel.x <= right_bottom.x; ++pixel.x)
		{
			set_pixel(matrix, pixel, color);
		}
	}
}