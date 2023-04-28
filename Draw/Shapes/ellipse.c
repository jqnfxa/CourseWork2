#include "ellipse.h"
#include "../../Validator/validator.h"
#include "../image.h"
#include <math.h>
#include <stdlib.h>

void draw_generic_ellipse(Matrix *matrix, int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t color, bool filled)
{
	int32_t a = abs(x1 - x0), b = abs(y1 - y0), b1 = b & 1;   /* values of diameter */
	long dx = 4 * (1 - a) * b * b, dy = 4 * (b1 + 1) * a * a;  /* error increment */
	long err = dx + dy + b1 * a * a, e2;                      /* error of 1.step */

	if(x0 > x1)
	{
		x0 = x1;
		x1 += a;
	}
	/* if called with swapped points */
	if(y0 > y1)
		y0 = y1; /* .. exchange them */
	y0 += (b + 1) / 2;
	y1 = y0 - b1; /* starting pixel */
	a *= 8 * a;
	b1 = 8 * b * b;

	do
	{
		if(filled)
		{
			xLine(matrix, x0, x1, y0, color);
			xLine(matrix, x0, x1, y1, color);
		}
		else
		{
			set_pixel(matrix, x1, y0, color);
			set_pixel(matrix, x0, y0, color);
			set_pixel(matrix, x0, y1, color);
			set_pixel(matrix, x1, y1, color);
		}
		e2 = 2 * err;
		if(e2 <= dy)
		{
			y0++;
			y1--;
			err += dy += a;
		} /* y step */
		if(e2 >= dx || 2 * err > dy)
		{
			x0++;
			x1--;
			err += dx += b1;
		} /* x step */
	} while(x0 <= x1);

	while(y0 - y1 < b)
	{
		/* too early stop of flat ellipses a=1 */

		if(filled)
		{
			xLine(matrix, x0 - 1, x1 + 1, y0++, color);
			xLine(matrix, x0 - 1, x1 + 1, y1--, color);
		}
		else
		{
			set_pixel(matrix, x0 - 1, y0, color);
			set_pixel(matrix, x1 + 1, y0++, color);
			set_pixel(matrix, x0 - 1, y1, color);
			set_pixel(matrix, x1 + 1, y1--, color);
		}
	}
}

void draw_wide_ellipse(Matrix *matrix, int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t width, int32_t color)
{
	--width;
	if(!width)
	{
		draw_generic_ellipse(matrix, x0, y0, x1, y1, color, 0);
		return;
	}

	for(int i = 0; i < width; ++i)
	{
		draw_generic_ellipse(matrix, x0 + i, y0 + i, x1 - i, y1 - i, color, 0);
		draw_generic_ellipse(matrix, x0 + i + 1, y0 + i, x1 - i, y1 - i, color, 0);
		draw_generic_ellipse(matrix, x0 + i, y0 + i, x1 - i - 1, y1 - i, color, 0);
		draw_generic_ellipse(matrix, x0 + i, y0 + i + 1, x1 - i, y1 - i, color, 0);
		draw_generic_ellipse(matrix, x0 + i, y0 + i, x1 - i, y1 - i - 1, color, 0);
		//draw_generic_ellipse(matrix, x0 + width + 1, y0 + width + 1, x1 - width - 1, y1 - width - 1, color, 0);
	}
}

void xLine(Matrix *matrix, int32_t x1, int32_t x2, int32_t y, int32_t color)
{
	if(!is_valid_matrix(matrix))
	{
		return;
	}
	while(x1 <= x2)
	{
		set_pixel(matrix, x1, y, color);
		++x1;
	}
}

void yLine(Matrix *matrix, int32_t x, int32_t y1, int32_t y2, int32_t color)
{
	if(!is_valid_matrix(matrix))
	{
		return;
	}
	while(y1 <= y2)
	{
		set_pixel(matrix, x, y1, color);
		++y1;
	}
}