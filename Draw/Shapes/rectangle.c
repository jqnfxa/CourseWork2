#include "rectangle.h"
#include "../../Validator/validator.h"
#include "../image.h"
#include <stddef.h>

void draw_rectangle(Matrix *matrix, RectangleQuery *info)
{
	if(!is_valid_matrix(matrix) || info == NULL)
	{
		return;
	}
	// image will be out of bounds any way
	if(!validate_image_area(matrix->width, matrix->height, &info->area))
	{
		return;
	}

	Point diff = {info->area.right_bottom.x - info->area.left_up.x,
				  info->area.right_bottom.y - info->area.left_up.y};

	// width is too big (no fill color possible)
	if(info->width > max((diff.x + 1) / 2, (diff.y + 1) / 2))
	{
		draw_filled_rectangle(matrix, &info->area, info->color);
		return;
	}

	Area drawing_area = {.left_up = info->area.left_up,
						 .right_bottom = (Point){info->area.right_bottom.x, info->area.left_up.y + info->width - 1}};
	// upper area
	draw_filled_rectangle(matrix, &drawing_area, info->color);

	drawing_area.left_up = (Point){info->area.left_up.x, info->area.right_bottom.y - info->width + 1};
	drawing_area.right_bottom = info->area.right_bottom;

	// bottom area
	draw_filled_rectangle(matrix, &drawing_area, info->color);

	drawing_area.left_up = (Point){info->area.left_up.x, info->area.left_up.y + info->width - 1},
	drawing_area.right_bottom = (Point){info->area.left_up.x + info->width - 1, info->area.right_bottom.y - info->width + 1};

	// left
	draw_filled_rectangle(matrix, &drawing_area, info->color);

	drawing_area.left_up = (Point){info->area.right_bottom.x - info->width + 1, info->area.left_up.y + info->width - 1},
	drawing_area.right_bottom = (Point){info->area.right_bottom.x, info->area.right_bottom.y - info->width + 1};

	// right
	draw_filled_rectangle(matrix, &drawing_area, info->color);

	if(!match_flags(info->check_sum, FILL))
	{
		return;
	}

	int32_t width_y = min(info->width, (diff.y + 1) / 2);
	int32_t width_x = min(info->width, (diff.x + 1) / 2);

	drawing_area.left_up = (Point){info->area.left_up.x + width_x - 1, info->area.left_up.y + width_y - 1};
	drawing_area.right_bottom = (Point){info->area.right_bottom.x - width_x + 1, info->area.right_bottom.y - width_y + 1};

	draw_filled_rectangle(matrix, &drawing_area, info->fill_color);
}

void draw_filled_rectangle(Matrix *matrix, Area *area, int32_t color)
{
	if(!is_valid_matrix(matrix) || !validate_area(area))
	{
		return;
	}
	// check if we actually need to draw something
	if(!validate_image_area(matrix->width, matrix->height, area))
	{
		return;
	}

	shrink_to_fit(matrix->width, matrix->height, area);

	for(int32_t y = area->left_up.y; y <= area->right_bottom.y; ++y)
	{
		for(int32_t x = area->left_up.x; x <= area->right_bottom.x; ++x)
		{
			set_pixel(matrix, x, y, color);
		}
	}
}