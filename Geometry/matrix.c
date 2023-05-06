#include "matrix.h"
#include "../ExceptionHandler/logger.h"
#include "../Validator/validator.h"
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

Matrix create_filled(int32_t rows, int32_t columns, int32_t color)
{
	Matrix matrix = create(rows, columns);

	fill(&matrix, color);

	return matrix;
}

void fill(Matrix *matrix, int32_t color)
{
	if(!is_valid_matrix(matrix) || !is_valid_rgb(color))
	{
		return;
	}

	unsafe_fill(matrix, color);

	for(int32_t i = 0; i < matrix->height; ++i)
	{
		for(int32_t j = 0; j < matrix->width; ++j)
		{
			matrix->grid[i][j] = color;
		}
	}
}

void unsafe_fill(Matrix *matrix, int32_t color)
{
	if(!is_valid_matrix(matrix))
	{
		return;
	}

	for(int32_t i = 0; i < matrix->height; ++i)
	{
		for(int32_t j = 0; j < matrix->width; ++j)
		{
			matrix->grid[i][j] = color;
		}
	}
}

void copy_additional(Matrix *dst, Matrix *src, const int32_t *avoid_colors, int32_t size)
{
	if(dst == NULL || src == NULL ||
	   dst->height != src->height || dst->width != src->width ||
	   avoid_colors == NULL)
	{
		return;
	}

	bool copyable = true;

	for(int32_t i = 0; i < dst->height; ++i)
	{
		for(int32_t j = 0; j < dst->width; ++j)
		{
			copyable = true;
			for(int32_t k = 0; k < size; ++k)
			{
				if(src->grid[i][j] == avoid_colors[k])
				{
					copyable = false;
					break;
				}
			}
			if(copyable)
			{
				dst->grid[i][j] = src->grid[i][j];
			}
		}
	}
}

void destroy(Matrix *matrix)
{
	if(!is_valid_matrix(matrix))
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
	if(!is_valid_matrix(matrix))
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
	if(!is_valid_matrix(matrix))
	{
		return;
	}
	flip_horizontal(matrix);
	flip_vertical(matrix);
}

void flip_horizontal(Matrix *matrix)
{
	if(!is_valid_matrix(matrix) || matrix->height == 0)
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
	if(!is_valid_matrix(matrix) || matrix->width == 0)
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