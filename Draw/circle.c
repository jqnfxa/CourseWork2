#include "circle.h"
#include "../Validator/validator.h"
#include "image.h"
#include <stddef.h>

//TODO small circle ( < 9*9) precision?
void draw_circle(Matrix *matrix, CircleQuery *info)
{
	if(matrix == NULL || info == NULL)
	{
		return;
	}

	if(get_circle_type(info) == 2)
	{
		info->radius = (info->area.right_bottom.x - info->area.left_up.x) / 2;
		info->center.x = info->area.left_up.x + (info->area.right_bottom.x - info->area.left_up.x) / 2;
		info->center.y = info->area.left_up.y + (info->area.right_bottom.y - info->area.left_up.y) / 2;
	}
	else
	{
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

	if(info->width >= info->radius)
	{
		info->width = info->radius;
	}

	draw_circle_v1(matrix, info->center, info->radius, info->radius - info->width, info->color);

	// check if we need to fill inner area
	if(match_flags(info->check_sum, FILL) && info->width < info->radius)
	{
		draw_circle_v1(matrix, info->center, info->radius - info->width, 0, info->fill_color);
	}
}

void xLine(Matrix *matrix, int32_t x1, int32_t x2, int32_t y, int32_t color)
{
	if(matrix == NULL)
	{
		return;
	}
	Point pixel = {x1, y};
	while(pixel.x <= x2)
	{
		set_pixel(matrix, &pixel, color);
		++pixel.x;
	}
}

void yLine(Matrix *matrix, int32_t x, int32_t y1, int32_t y2, int32_t color)
{
	if(matrix == NULL)
	{
		return;
	}
	Point pixel = {x, y1};
	while(pixel.y <= y2)
	{
		set_pixel(matrix, &pixel, color);
		++pixel.y;
	}
}

void draw_circle_v1(Matrix *matrix, Point center, int32_t outer, int32_t inner, int32_t color)
{
	if(matrix == NULL)
	{
		return;
	}

	int32_t x0 = outer;
	int32_t xi = inner;
	int32_t y = 0;
	int32_t erro = 1 - x0;
	int32_t erri = 1 - xi;

	while(x0 >= y)
	{
		xLine(matrix, center.x + xi, center.x + x0, center.y + y, color);
		yLine(matrix, center.x + y, center.y + xi, center.y + x0, color);
		xLine(matrix, center.x - x0, center.x - xi, center.y + y, color);
		yLine(matrix, center.x - y, center.y + xi, center.y + x0, color);
		xLine(matrix, center.x - x0, center.x - xi, center.y - y, color);
		yLine(matrix, center.x - y, center.y - x0, center.y - xi, color);
		xLine(matrix, center.x + xi, center.x + x0, center.y - y, color);
		yLine(matrix, center.x + y, center.y - x0, center.y - xi, color);

		y++;

		if(erro < 0)
		{
			erro += 2 * y + 1;
		}
		else
		{
			x0--;
			erro += 2 * (y - x0 + 1);
		}
		if(y > inner)
		{
			xi = y;
		}
		else
		{
			if(erri < 0)
			{
				erri += 2 * y + 1;
			}
			else
			{
				xi--;
				erri += 2 * (y - xi + 1);
			}
		}
	}
}