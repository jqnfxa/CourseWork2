#include "frame.h"
#include "../Geometry/matrix.h"
#include "../Validator/validator.h"
#include "Shapes/circle.h"
#include "image.h"
#include <string.h>

void complete_frame_query(Matrix *matrix, FrameQuery *query)
{
	if(matrix == NULL || matrix->grid == NULL || query == NULL)
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
	if(src == NULL || *src == NULL || (*src)->grid == NULL || !is_valid_rgb(color) || frame_width < 1)
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
	Matrix matrix = create((*src)->height, (*src)->width);

	Point pp = {0, 0};
	paste(&matrix, *src, &pp);

	destroy(*src);

	**src = create(matrix.height + 2 * frame_width, matrix.width + 2 * frame_width);

	int32_t inverted_color = invert_color(color);

	fill(*src, inverted_color);

	int32_t radius = (*src)->height / 35 + 1;

	CircleQuery query;
	query.radius = radius;
	query.width = radius / 10 + 1;
	query.color = color;
	query.check_sum = 0;
	set_flags(&query.check_sum, POINT | RADIUS | WIDTH | COLOR);

	for(int32_t y = 0; y < (*src)->height + (*src)->width; y += 8 * radius)
	{
		for(int32_t x = 0; x < (*src)->width + (*src)->width; x += 8 * radius)
		{
			query.center.x = x;
			query.center.y = y;

			draw_circle(*src, &query);
		}
	}

	query.radius /= 2;
	query.radius++;

	for(int32_t y = 0; y < (*src)->height + (*src)->width; y += 4 * radius)
	{
		for(int32_t x = 0; x < (*src)->width + (*src)->width; x += 4 * radius)
		{
			query.center.x = x;
			query.center.y = y;

			draw_circle(*src, &query);
		}
	}

	query.radius *= 4;

	for(int32_t y = 0; y < (*src)->height + (*src)->width; y += 2 * radius)
	{
		for(int32_t x = 0; x < (*src)->width + (*src)->width; x += 2 * radius)
		{
			query.center.x = x;
			query.center.y = y;

			draw_circle(*src, &query);
		}
	}

	pp.x = frame_width;
	pp.y = frame_width;
	paste(*src, &matrix, &pp);

	destroy(&matrix);
}