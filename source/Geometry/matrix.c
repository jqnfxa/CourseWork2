#include "../../interfaces/Geometry/matrix.h"
#include "../../interfaces/CLI/types_parser.h"
#include <stdlib.h>

void swap(int32_t *lhs, int32_t *rhs)
{
	if(lhs == NULL || rhs == NULL)
	{
		return;
	}
	int32_t temp = *lhs;
	*lhs = *rhs;
	*rhs = temp;
}

Matrix create(size_t n, size_t m)
{
	if(n <= 0 || m <= 0)
	{
		return (Matrix){};
	}

	Matrix matrix;

	matrix.height = n;
	matrix.width = m;

	matrix.grid = (int32_t **)malloc(sizeof(int32_t *) * n);

	if(matrix.grid == NULL)
	{
		return (Matrix){};
	}

	for(size_t i = 0; i < matrix.height; ++i)
	{
		matrix.grid[i] = (int32_t *)calloc(m, sizeof(int));

		if(matrix.grid[i] == NULL)
		{
			matrix.height = (i == 0 ? 0 : i - 1);
			destroy(&matrix);
			return (Matrix){};
		}
	}
	return matrix;
}

Matrix crop(Matrix *matrix, Point left_up, Point right_bottom)
{
	if(matrix == NULL ||
	   left_up.x > right_bottom.x ||
	   left_up.y > right_bottom.y ||
	   left_up.x >= (int)matrix->width || left_up.y >= (int)matrix->height)
	{
		return (Matrix){};
	}

	if(left_up.x < 0)
	{
		left_up.x = 0;
	}
	if(left_up.y < 0)
	{
		left_up.y = 0;
	}
	if(right_bottom.x > matrix->width)
	{
		right_bottom.x = (int)matrix->width;
	}
	if(right_bottom.y > matrix->height)
	{
		right_bottom.y = (int)matrix->height;
	}

	Matrix sub = create(right_bottom.y - left_up.y, right_bottom.x - left_up.x);

	for(size_t i = left_up.y, row = 0; i < right_bottom.y; ++i, ++row)
	{
		for(size_t j = left_up.x, column = 0; j < right_bottom.x; ++j, ++column)
		{
			sub.grid[row][column] = matrix->grid[i][j];
		}
	}
	return sub;
}

void paste(Matrix *dst, Matrix *src, Point left_up)
{
	if(dst == NULL || src == NULL ||
	   left_up.x >= (int)dst->width || left_up.y >= (int)dst->height ||
	   left_up.y + src->height < 0 || left_up.x + src->width < 0)
	{
		return;
	}

	Point pixel = {left_up.x, left_up.y};

	for(size_t i = 0; i < src->height; ++i, ++pixel.y)
	{
		pixel.x = left_up.x;
		for(size_t j = 0; j < src->width; ++j, ++pixel.x)
		{
			set_pixel(dst, pixel, src->grid[i][j]);
		}
	}
}

void destroy(Matrix *matrix)
{
	if(matrix == NULL)
	{
		return;
	}
	for(size_t i = 0; i < matrix->height; ++i)
	{
		free(matrix->grid[i]);
	}
	free(matrix->grid);

	// zeroing
	matrix->grid = NULL;
	matrix->height = matrix->width = 0;
}

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

Matrix transpose(Matrix *matrix)
{
	if(matrix == NULL)
	{
		return (Matrix){};
	}

	Matrix transposed = create(matrix->width, matrix->height);

	for(size_t i = 0; i < matrix->height; ++i)
	{
		for(size_t j = 0; j < matrix->width; ++j)
		{
			transposed.grid[j][i] = matrix->grid[i][j];
		}
	}
	return transposed;
}

Matrix rotate_left(Matrix matrix)
{
	Matrix result = transpose(&matrix);
	flip_horizontal(&result);

	return result;
}

Matrix rotate_right(Matrix matrix)
{
	Matrix result = transpose(&matrix);
	flip_vertical(&result);

	return result;
}

void flip_horizontal(Matrix *matrix)
{
	if(matrix == NULL || matrix->height == 0)
	{
		return;
	}

	for(size_t i = 0, j = matrix->height - 1; i < matrix->height / 2; ++i, j--)
	{
		for(size_t column = 0; column < matrix->width; ++column)
		{
			swap(&matrix->grid[i][column], &matrix->grid[j][column]);
		}
	}
}

void flip_vertical(Matrix *matrix)
{
	if(matrix == NULL || matrix->width == 0)
	{
		return;
	}

	for(size_t i = 0, j = matrix->width - 1; i < matrix->width / 2; ++i, j--)
	{
		for(size_t row = 0; row < matrix->height; ++row)
		{
			swap(&matrix->grid[row][i], &matrix->grid[row][j]);
		}
	}
}