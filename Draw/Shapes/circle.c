#include "circle.h"
#include "../../Validator/validator.h"
#include "ellipse.h"
#include "../image.h"
#include <stddef.h>
#include <stdlib.h>

void draw_circle(Matrix *matrix, CircleQuery *info)
{
	if(!is_valid_matrix(matrix) || info == NULL)
	{
		return;
	}
	if(get_circle_type(info) == 1)
	{
		if(info->radius == 0)
		{
			set_pixel(matrix, info->center.x, info->center.y, info->color);
			return;
		}

		info->area.left_up.x = info->center.x - info->radius;
		info->area.left_up.y = info->center.y - info->radius;
		info->area.right_bottom.x = info->center.x + info->radius;
		info->area.right_bottom.y = info->center.y + info->radius;
	}

	// nothing to draw
	if(!validate_image_area(matrix->width, matrix->height, &info->area))
	{
		return;
	}

	int32_t max_width = (min(abs(info->area.left_up.x - info->area.right_bottom.x), abs(info->area.left_up.y - info->area.right_bottom.y)) + 1) / 2;

	if(info->width > max_width)
	{
		set_flags(&info->check_sum, FILL);
		info->fill_color = info->color;
	}

	// check if we need to fill inner area
	if(match_flags(info->check_sum, FILL))
	{
		draw_generic_ellipse(matrix, info->area.left_up.x, info->area.left_up.y, info->area.right_bottom.x, info->area.right_bottom.y, info->fill_color, 1);
	}

	draw_wide_ellipse(matrix, info->area.left_up.x, info->area.left_up.y, info->area.right_bottom.x, info->area.right_bottom.y, info->width, info->color);
}