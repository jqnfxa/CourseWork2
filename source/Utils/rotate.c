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
		switch(info->direction)
		{
			case LEFT: {
				Matrix sub = crop(matrix, info->left_up, info->right_bottom);
				Matrix rotated = rotate_left(sub);

				Point paste_point = {info->left_up.x + (info->right_bottom.x - info->left_up.x) / 2 - (int)sub.height / 2,
								info->left_up.y + (info->right_bottom.y - info->left_up.y) / 2 - (int)sub.width / 2};
				paste(matrix, &rotated, paste_point);
				destroy(&rotated);
			}
			break;
			case RIGHT:{
				Matrix sub = crop(matrix, info->left_up, info->right_bottom);
				Matrix rotated = rotate_right(sub);

				Point paste_point = {info->left_up.x + (int)sub.width / 2  - (int)sub.height / 2,
									 info->left_up.y + (int)sub.height / 2 - (int)sub.width / 2};
				paste(matrix, &rotated, paste_point);
				destroy(&rotated);
			}
			break;
			case INVALID:
			default:
				break;
		}
	}
}