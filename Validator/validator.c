#include "validator.h"
#include <stddef.h>
#include <string.h>

bool match_postfix(const char *filename, const char *needle)
{
	if(filename == NULL)
	{
		return false;
	}

	if(strlen(filename) < strlen(needle))
	{
		return false;
	}

	// calculate offset to match
	size_t offset = strlen(filename) - strlen(needle);

	return !strcmp(filename + offset, needle);
}

bool match_flags(int32_t value, int32_t flags)
{
	return (value & flags) == flags;
}

void set_flags(int32_t *value, int32_t flags)
{
	if(value == NULL)
	{
		return;
	}
	*value |= flags;
}

void shrink_to_fit(int32_t width, int32_t height, Area *area)
{
	if(area == NULL)
	{
		return;
	}
	if(area->left_up.x < 0)
	{
		area->left_up.x = 0;
	}
	if(area->left_up.y < 0)
	{
		area->left_up.y = 0;
	}
	if(area->left_up.x > width)
	{
		area->left_up.x = width;
	}
	if(area->left_up.y > height)
	{
		area->left_up.y = height;
	}
}

bool is_in_bound(Matrix *matrix, Point *position)
{
	if(matrix == NULL || position == NULL)
	{
		return false;
	}

	// check x
	if(position->x < 0 || position->x >= matrix->width)
	{
		return false;
	}
	// check y
	if(position->y < 0 || position->y >= matrix->height)
	{
		return false;
	}

	return true;
}

bool is_valid_rgb(int color)
{
	if(color < 0 || color > 0xffffff)
	{
		return false;
	}
	return true;
}

bool validate_area(Area *area)
{
	if(area == NULL)
	{
		return false;
	}
	if(area->left_up.x > area->right_bottom.x || area->left_up.y > area->right_bottom.y)
	{
		return false;
	}
	return true;
}

bool is_square_area(Area *area)
{
	if(!validate_area(area))
	{
		return false;
	}

	return (area->right_bottom.x - area->left_up.x) == (area->right_bottom.y - area->left_up.y);
}

bool validate_image_area(int32_t width, int32_t height, Area *area)
{
	if(!validate_area(area))
	{
		return false;
	}
	if(area->left_up.x >= width || area->left_up.y >= height || area->right_bottom.x < 0 || area->right_bottom.y < 0)
	{
		return false;
	}
	return true;
}

bool is_valid_bmp(const char *file_name)
{
	if(file_name == NULL)
	{
		return false;
	}
	size_t file_len = strlen(file_name);
	if(file_len < 5 || file_len > 255)
	{
		return false;
	}

	return match_postfix(file_name, ".bmp");
}

bool validate_rectangle(RectangleQuery *query)
{
	if(query == NULL)
	{
		return false;
	}
	// validate coordinates
	if(!match_flags(query->check_sum, START_POINT | END_POINT))
	{
		//TODO log_error(MISSING_ARGUMENT, "--start | --end");
		return false;
	}
	if(!validate_area(&query->area))
	{
		//TODO log_error(CONVERSATION_ERROR, "--start | --end");
		return false;
	}

	// validate color
	if(!match_flags(query->check_sum, COLOR))
	{
		//TODO log_error(MISSING_ARGUMENT, "--color");
		return false;
	}
	if(!is_valid_rgb(query->color))
	{
		//TODO log_error(CONVERSATION_ERROR, "--color");
		return false;
	}

	// check if we have override width
	if(match_flags(query->check_sum, WIDTH))
	{
		if(query->width <= 0)
		{
			//TODO log_error(CONVERSATION_ERROR, "--width");
			return false;
		}
	}
	else
	{
		query->width = 1;
	}

	// check if we have fill
	if(match_flags(query->check_sum, FILL))
	{
		if(!is_valid_rgb(query->fill_color))
		{
			//TODO log_error(CONVERSATION_ERROR, "--fill");
			return false;
		}
	}
	return true;
}

int16_t get_circle_type(CircleQuery *request)
{
	if(request == NULL)
	{
		return 0;
	}

	// looking for area
	if(match_flags(request->check_sum, START_POINT | END_POINT))
	{
		// we got point or radius ?
		if(match_flags(request->check_sum, POINT) || match_flags(request->check_sum, RADIUS))
		{
			//TODO log_error(AMBIGUOUS_CALL, "--circle");
		}
		else
		{
			return 2;
		}
	}
	else
	{
		// we got area ?
		if(match_flags(request->check_sum, START_POINT) || match_flags(request->check_sum, END_POINT))
		{
			//TODO log_error(AMBIGUOUS_CALL, "--circle");
		}
		else
		{
			return 1;
		}
	}
	// invalid type
	return 0;
}

bool validate_circle(CircleQuery *query)
{
	if(query == NULL)
	{
		return false;
	}

	switch(get_circle_type(query))
	{
		case 1: {
			if(query->radius <= 0)
			{
				//TODO log_error(CONVERSATION_ERROR, "--radius");
				return false;
			}
		}
		break;
		case 2: {
			if(!is_square_area(&query->area))
			{
				//TODO log_error(CONVERSATION_ERROR, "--start | --end");
				return false;
			}
		}
		break;
		case 0:
		default:
			//TODO log_error(MISSING_ARGUMENT, "--circle");
			return false;
	}

	// validate color
	if(!match_flags(query->check_sum, COLOR))
	{
		//TODO log_error(MISSING_ARGUMENT, "--color");
		return false;
	}
	if(!is_valid_rgb(query->color))
	{
		//TODO log_error(CONVERSATION_ERROR, "--color");
		return false;
	}

	// check if we have override width
	if(match_flags(query->check_sum, WIDTH))
	{
		if(query->width <= 0)
		{
			//TODO log_error(CONVERSATION_ERROR, "--width");
			return false;
		}
	}
	else
	{
		query->width = 1;
	}

	// check if we have fill
	if(match_flags(query->check_sum, FILL))
	{
		if(!is_valid_rgb(query->fill_color))
		{
			//TODO log_error(CONVERSATION_ERROR, "--fill");
			return false;
		}
	}

	return true;
}

bool validate_frame(FrameQuery *query)
{
	if(query == NULL)
	{
		return false;
	}
	// check type
	if(!match_flags(query->check_sum, FRAME_TYPE))
	{
		//TODO log_error(MISSING_ARGUMENT, "--type");
		return false;
	}
	// check color
	if(!match_flags(query->check_sum, COLOR))
	{
		//TODO log_error(MISSING_ARGUMENT, "--color");
		return false;
	}
	if(!is_valid_rgb(query->color))
	{
		//TODO log_error(CONVERSATION_ERROR, "--color");
		return false;
	}

	// check for width override
	if(match_flags(query->check_sum, WIDTH))
	{
		if(query->width <= 0)
		{
			//TODO log_error(CONVERSATION_ERROR, "--width");
			return false;
		}
	}
	else
	{
		query->width = 1;
	}

	return true;
}

bool validate_rotate(RotateQuery *query)
{
	if(query == NULL)
	{
		return false;
	}
	// validate coordinates
	if(!match_flags(query->check_sum, START_POINT | END_POINT))
	{
		//TODO log_error(MISSING_ARGUMENT, "--start | --end");
		return false;
	}
	if(!validate_area(&query->area))
	{
		//TODO log_error(CONVERSATION_ERROR, "--start | --end");
		return false;
	}
	// check angle
	if(!match_flags(query->check_sum, ANGLE))
	{
		//TODO log_error(MISSING_ARGUMENT, "--angle");
		return false;
	}
	else
	{
		if(query->angle != 90 && query->angle != 180 && query->angle != 270)
		{
			//TODO log_error(CONVERSATION_ERROR, "--angle");
			return false;
		}
	}

	// check direction
	if(query->direction == INVALID)
	{
		//TODO log_error(CONVERSATION_ERROR, "--direction");
		return false;
	}
	return true;
}