#include "frame.h"
#include "../Geometry/matrix.h"
#include "../Validator/validator.h"
#include "Shapes/ellipse.h"
#include "Shapes/polygon.h"
#include "image.h"
#include <math.h>
#include <string.h>

void complete_frame_query(Matrix *matrix, FrameQuery *query)
{
	if(!is_valid_matrix(matrix) || query == NULL)
	{
		return;
	}
	if(!match_flags(query->check_sum, WIDTH))
	{
		query->width = min(matrix->width, matrix->height) / 15 + 1;
	}
	switch(query->type)
	{
		case 1:
			gen_simple_lines(&matrix, query->width, query->color);
			break;
		case 2:
			gen_simple_circles(&matrix, query->width, query->color);
			break;
		default:
			break;
	}
}

void gen_simple_lines(Matrix **src, int32_t frame_width, int32_t color)
{
	if(src == NULL || !is_valid_matrix(*src) || !is_valid_rgb(color) || frame_width < 1)
	{
		return;
	}

	Matrix matrix = create((*src)->height, (*src)->width);

	Point pp = {0, 0};
	paste(&matrix, *src, &pp);

	destroy(*src);

	**src = create(matrix.height + 2 * frame_width, matrix.width + 2 * frame_width);

	int32_t inverted_color = invert_color(color);

	fill(*src, inverted_color);

	int32_t line_width = min(matrix.width, matrix.height) / 100 + frame_width / 25 + 1;

	for(int32_t i = 0; i < (*src)->height + (*src)->width; i += line_width * 2)
	{
		for(int32_t k = 0, j = i; k < (*src)->width; ++k, --j)
		{
			for(int32_t m = 0; m < line_width; ++m)
			{
				set_pixel(*src, k, j - m, color);
			}
		}
	}

	for(int32_t i = -(*src)->width - (*src)->height; i < (*src)->height; i += line_width * 2)
	{
		for(int32_t k = 0, j = i; k < (*src)->width; ++k, ++j)
		{
			for(int32_t m = 0; m < line_width; ++m)
			{
				set_pixel(*src, k, j + m, color);
			}
		}
	}

	pp.x = frame_width;
	pp.y = frame_width;
	paste(*src, &matrix, &pp);

	destroy(&matrix);
}

void gen_simple_circles(Matrix **src, int32_t frame_width, int32_t color)
{
	if(src == NULL || !is_valid_matrix(*src) || !is_valid_rgb(color) || frame_width < 1)
	{
		return;
	}

	Matrix matrix = create((*src)->height, (*src)->width);

	Point pp = {0, 0};
	paste(&matrix, *src, &pp);

	destroy(*src);

	**src = create(matrix.height + 2 * frame_width, matrix.width + 2 * frame_width);

	int32_t inverted_color = invert_color(color);

	fill(*src, inverted_color);

	int32_t y0 = 0;
	int32_t y1 = (*src)->height;
	int32_t x0 = 0;
	int32_t x1 = (*src)->width;

	int32_t y_step = ceil((y1 - y0) * 1.0 / 27);
	int32_t x_step = ceil((x1 - x0) * 1.0 / 27);

	int width = ceil((y_step + x_step) * 1.0 / 15);

	for(int32_t y = y0; y < y1 + y_step; y += y_step * 3)
	{
		for(int32_t x = x0; x < x1 + x_step; x += x_step * 3)
		{
			draw_wide_ellipse(*src, x - x_step, y - y_step, x + x_step, y + y_step, width, color);
		}
	}

	for(int32_t y = ceil((y0 + y_step) * 1.5); y < y1 + y_step; y += y_step * 3)
	{
		for(int32_t x = ceil((x0 + x_step) * 1.5); x < x1 + x_step; x += x_step * 3)
		{
			draw_wide_ellipse(*src, x - 2 * x_step, y - 2 * y_step, x + 2 * x_step, y + 2 * y_step, width, color);
		}
	}

	for(int32_t y = ceil((y0 + y_step) * 1.5); y < y1 + y_step; y += y_step * 3)
	{
		for(int32_t x = ceil((x0 + x_step) * 1.5); x < x1 + x_step; x += x_step * 3)
		{
			draw_wide_line(*src, x0, y, x1, y, color, width, -1);
			draw_wide_line(*src, x, y0, x, y1, color, width, -1);
			draw_wide_ellipse(*src, x - x_step, y - y_step, x + x_step, y + y_step, width, color);
		}
	}

	pp.x = frame_width;
	pp.y = frame_width;
	paste(*src, &matrix, &pp);

	destroy(&matrix);
}