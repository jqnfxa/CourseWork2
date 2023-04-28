#include "rotate.h"
#include "image.h"
#include "../Validator/validator.h"
#include <math.h>
#include <stddef.h>

Matrix rotate_advanced(Matrix *matrix, double angle);

void rotate_area(Matrix *matrix, RotateQuery *info)
{
	if(!is_valid_matrix(matrix) || info == NULL)
	{
		return;
	}

	if(info->angle < 0)
	{
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
		int angle;
		Matrix area_to_rotate = crop(matrix, &info->area);

		if(info->angle < 90)
		{
			angle = info->angle * (info->direction == LEFT ? -1 : 1);
		}
		else if(info->angle < 270)
		{
			rotate_180(&area_to_rotate);

			angle = (180 - info->angle) * (info->direction == LEFT ? 1 : -1);
		}
		else
		{
			angle = (360 - info->angle) * (info->direction == LEFT ? 1 : -1);
		}

		Matrix rotated = rotate_advanced(&area_to_rotate, angle * (M_PI / 180));

		Point pp = {(info->area.right_bottom.x + info->area.left_up.x) / 2 -  rotated.width / 2,
					(info->area.right_bottom.y + info->area.left_up.y) / 2 -  rotated.height / 2};

		paste_if(matrix, &rotated, &pp, -1);

		destroy(&rotated);
		destroy(&area_to_rotate);
	}
}

void shear(double angle, int32_t *x, int32_t *y)
{
		double tangent = tan(angle / 2);
		int32_t new_x = (int32_t)round(*x - *y * tangent);
		int32_t new_y = *y;

		new_y = (int32_t)round(new_x * sin(angle) + new_y);
		new_x = (int32_t)round(new_x - new_y * tangent);

		*x = new_x;
		*y = new_y;
}

Matrix rotate_advanced(Matrix *matrix, double angle)
{
	double cosine = cos(angle);
	double sine = sin(angle);

	int32_t height = matrix->height;
	int32_t width = matrix->width;

	int32_t new_height = (int32_t)round(fabs(height * cosine) + fabs(width * sine)) + 1;
	int32_t new_width = (int32_t)round(fabs(width * cosine) + fabs(height * sine)) + 1;

	Matrix output = create(new_height, new_width);

	for(int32_t i = 0; i < output.height; ++i)
	{
		for(int32_t j = 0; j < output.width; ++j)
		{
			output.grid[i][j] = -1;
		}
	}

	int32_t original_centre_height = (int32_t)round(((height + 1) * 1.0 / 2) - 1);
	int32_t original_centre_width = (int32_t)round(((width + 1) * 1.0 / 2) - 1);

	int32_t new_centre_height = (int32_t)round(((new_height + 1) * 1.0 / 2) - 1);
	int32_t new_centre_width = (int32_t)round(((new_width + 1)  * 1.0 / 2) - 1);

	int32_t x, y, new_x, new_y;

	for(int32_t i = 0; i < height; ++i)
	{
		for(int32_t j = 0; j < width; ++j)
		{
			y = height - 1 - i - original_centre_height;
			x = width - 1 - j - original_centre_width;

			shear(angle, &x, &y);

			new_y = new_centre_height - y;
			new_x = new_centre_width - x;

			set_pixel(&output, new_x, new_y, matrix->grid[i][j]);
		}
	}
	return output;
}