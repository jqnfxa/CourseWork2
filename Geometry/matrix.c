#include "matrix.h"
#include "../ExceptionHandler/logger.h"
#include <stddef.h>
#include <stdlib.h>

Matrix create(int32_t rows, int32_t columns)
{
	if(rows <= 0 || columns <= 0)
	{
		return (Matrix){};
	}

	Matrix matrix = {.height = rows, .width = columns};
	matrix.grid = (int32_t **)malloc(sizeof(int32_t *) * matrix.height);

	if(matrix.grid == NULL)
	{
		log_error(OUT_OF_MEMORY, "create matrix");
		return (Matrix){};
	}
	for(int32_t i = 0; i < matrix.height; ++i)
	{
		matrix.grid[i] = (int32_t *)calloc(matrix.width, sizeof(int32_t));

		if(matrix.grid[i] == NULL)
		{
			log_error(OUT_OF_MEMORY, "create matrix");
			matrix.height = (i == 0 ? 0 : i - 1);
			destroy(&matrix);
			return (Matrix){};
		}
	}
	return matrix;
}

void destroy(Matrix *matrix)
{
	if(matrix == NULL)
	{
		return;
	}
	for(int32_t i = 0; i < matrix->height; ++i)
	{
		free(matrix->grid[i]);
	}
	free(matrix->grid);

	// zeroing
	matrix->grid = NULL;
	matrix->height = matrix->width = 0;
}

Matrix transpose(Matrix *matrix)
{
	if(matrix == NULL)
	{
		return (Matrix){};
	}

	Matrix transposed = create(matrix->width, matrix->height);

	for(int32_t i = 0; i < matrix->height; ++i)
	{
		for(int32_t j = 0; j < matrix->width; ++j)
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

void rotate_180(Matrix *matrix)
{
	if(matrix == NULL)
	{
		return;
	}
	flip_horizontal(matrix);
	flip_vertical(matrix);
}

void flip_horizontal(Matrix *matrix)
{
	if(matrix == NULL || matrix->height == 0)
	{
		return;
	}
	for(int32_t i = 0, j = matrix->height - 1; i < matrix->height / 2; ++i, --j)
	{
		for(int32_t column = 0; column < matrix->width; ++column)
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
	for(int32_t i = 0, j = matrix->width - 1; i < matrix->width / 2; ++i, j--)
	{
		for(int32_t row = 0; row < matrix->height; ++row)
		{
			swap(&matrix->grid[row][i], &matrix->grid[row][j]);
		}
	}
}

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