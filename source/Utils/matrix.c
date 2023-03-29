#include "../../interfaces/Utils/matrix.h"

void set_pixel(Matrix *matrix, Point pixel, int color)
{
	if(!is_in_bound(matrix, pixel))
	{
		return;
	}

	matrix->grid[pixel.y][pixel.x] = color;
}

bool is_in_bound(Matrix *matrix, Point position)
{
	if(matrix == NULL)
	{
		return false;
	}

	// check x
	if(position.x < 0 || position.x >= matrix->width)
	{
		return false;
	}
	// check y
	if(position.y < 0 || position.y >= matrix->height)
	{
		return false;
	}

	return true;
}