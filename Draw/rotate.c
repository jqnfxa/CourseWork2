#include "rotate.h"
#include "image.h"
#include <stddef.h>

void rotate_area(Matrix *matrix, RotateQuery *info)
{
	if(matrix == NULL || info == NULL)
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
}
