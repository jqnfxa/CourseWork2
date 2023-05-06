#include "rotate.h"
#include "../Validator/validator.h"
#include "image.h"
#include <math.h>
#include <stddef.h>

Matrix rotate_advanced(Matrix *matrix, double angle);

void rotate_area(Matrix *matrix, RotateQuery *info)
{
	if(!is_valid_matrix(matrix) || info == NULL)
	{
		return;
	}
	if(info->area.left_up.x == info->area.right_bottom.x && info->area.left_up.y == info->area.right_bottom.y)
	{
		return;
	}

	if(info->angle < 0)
	{
		if(info->angle == INT32_MIN)
		{
			// abs(INT32_MIN) % 360 == 128
			info->angle = -128;
		}
		info->angle *= -1;
		info->direction = (info->direction == LEFT ? RIGHT : LEFT);
	}

	info->angle %= 360;

	if(info->angle == 0)
	{
		return;
	}

	if(info->angle == 180)
	{
		Matrix sub = crop(matrix, &info->area);
		rotate_180(&sub);
		paste(matrix, &sub, &info->area.left_up);
		destroy(&sub);
	}
	else if(info->angle == 90 || info->angle == 270)
	{
		if(info->angle == 270)
		{
			info->direction = (info->direction == LEFT ? RIGHT : LEFT);
		}
		if(info->direction == LEFT || info->direction == RIGHT)
		{
			Matrix sub = crop(matrix, &info->area);
			Matrix rotated = (info->direction == LEFT ? rotate_left(sub) : rotate_right(sub));

			Point paste_point = {info->area.left_up.x + sub.width / 2 - sub.height / 2,
								 info->area.left_up.y + sub.height / 2 - sub.width / 2};
			paste(matrix, &rotated, &paste_point);
			destroy(&rotated);
			destroy(&sub);
		}
	}
	else
	{
		Matrix area_to_rotate = crop(matrix, &info->area);
		Matrix rotated = rotate_advanced(&area_to_rotate, -info->angle * (M_PI / 180));

		Point pp = {(info->area.right_bottom.x + info->area.left_up.x) / 2 -  rotated.width / 2,
					(info->area.right_bottom.y + info->area.left_up.y) / 2 -  rotated.height / 2};

		paste_if(matrix, &rotated, &pp, -1);

		destroy(&rotated);
		destroy(&area_to_rotate);
	}
}

Matrix rotate_advanced(Matrix *matrix, double angle)
{
	double cosine = cos(angle);
	double sine = sin(angle);

	Matrix output = create((int32_t)round(fabs(matrix->height * cosine)) +
							   (int32_t)round(fabs(matrix->width * sine)),
						   (int32_t)round(fabs(matrix->width * cosine)) +
							   (int32_t)round(fabs(matrix->height * sine)));

	unsafe_fill(&output, -1);

	int32_t cx = matrix->width / 2;
	int32_t cy = matrix->height / 2;

	int32_t midx = output.width / 2;
	int32_t midy = output.height / 2;

	int32_t x, y;

	for(int32_t i = 0; i < output.height; ++i)
	{
		for(int32_t j = 0; j < output.width; ++j)
		{
			x = (int32_t)round((j - midx) * cos(angle) + (i - midy) * sin(angle)) + cx;
			y = (int32_t)round((j - midx) * -sin(angle) + (i - midy) * cos(angle)) + cy;

			if(x >= 0 && y >= 0 && x < matrix->width && y < matrix->height)
			{
				output.grid[i][j] = matrix->grid[y][x];
			}
		}
	}
	return output;
}