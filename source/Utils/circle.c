#include "../../interfaces/Utils/circle.h"
#include "../../interfaces/CLI/types_parser.h"
#include "../../interfaces/Geometry/matrix.h"

//TODO small circle ( < 9*9) precision
void draw_circle(Matrix *matrix, CircleRequest *info)
{
	if(matrix == NULL || info == NULL)
	{
		return;
	}

	if(get_circle_type(info) == 2)
	{
		info->radius = (info->right_bottom.x - info->left_up.x) / 2;
		info->center.x = info->left_up.x + (info->right_bottom.x - info->left_up.x) / 2;
		info->center.y = info->left_up.y + (info->right_bottom.y - info->left_up.y) / 2;
	}
	else
	{
		info->left_up.x = info->center.x - info->radius;
		info->left_up.y = info->center.y - info->radius;
		info->right_bottom.x = info->center.x + info->radius;
		info->right_bottom.y = info->center.y + info->radius;
	}

	// nothing to draw
	if(is_out_of_bounds(matrix->width, matrix->height, info->left_up, info->right_bottom))
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

void xLine(Matrix *matrix, int x1, int x2, int y, int color)
{
	if(matrix == NULL)
	{
		return;
	}
	Point pixel = {x1, y};
	while(pixel.x <= x2)
	{
		set_pixel(matrix, pixel, color);
		++pixel.x;
	}
}

void yLine(Matrix *matrix, int x, int y1, int y2, int color)
{
	if(matrix == NULL)
	{
		return;
	}
	Point pixel = {x, y1};
	while(pixel.y <= y2)
	{
		set_pixel(matrix, pixel, color);
		++pixel.y;
	}
}

void draw_circle_v1(Matrix *matrix, Point center, int outer, int inner, int color)
{
	if(matrix == NULL)
	{
		return;
	}

	int x0 = outer;
	int xi = inner;
	int y = 0;
	int erro = 1 - x0;
	int erri = 1 - xi;

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