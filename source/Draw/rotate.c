#include "../../interfaces/Utils/rotate.h"

void rotate_area(Matrix *matrix, RotateRequest *info)
{
	if(matrix == NULL || info == NULL)
	{
		return;
	}

	if(info->angle == 180)
	{
		Matrix sub = crop(matrix, info->left_up, info->right_bottom);
		flip_horizontal(&sub);
		flip_vertical(&sub);
		paste(matrix, &sub, info->left_up);
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
			Matrix sub = crop(matrix, info->left_up, info->right_bottom);
			Matrix rotated = (info->direction == LEFT ? rotate_left(sub) : rotate_right(sub));

			Point paste_point = {info->left_up.x + sub.width / 2 - sub.height / 2,
								 info->left_up.y + sub.height / 2 - sub.width / 2};
			paste(matrix, &rotated, paste_point);
			destroy(&rotated);
			destroy(&sub);
		}
	}
}