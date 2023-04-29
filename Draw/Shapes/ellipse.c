#include "ellipse.h"
#include "../../Validator/validator.h"
#include "../image.h"
#include <math.h>
#include <stdint.h>
#include <stdlib.h>

void brezenham_ellipse(Matrix *matrix, int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t color, bool filled)
{
	int32_t a = abs(x1 - x0), b = abs(y1 - y0), b1 = b & 1;   /* values of diameter */
	long dx = 4 * (1 - a) * b * b, dy = 4 * (b1 + 1) * a * a; /* error increment */
	long err = dx + dy + b1 * a * a, e2;                      /* error of 1.step */

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

void ellipse_plot_points(Matrix *matrix, int32_t xc, int32_t yc, int32_t x, int32_t y, int32_t color, bool filled)
{
	if(filled)
	{
		xLine(matrix, xc - x, xc + x, yc + y, color);
		xLine(matrix, xc - x, xc + x, yc - y, color);
	}
	else
	{
		set_pixel(matrix, xc + x, yc + y, color);
		set_pixel(matrix, xc - x, yc + y, color);
		set_pixel(matrix, xc + x, yc - y, color);
		set_pixel(matrix, xc - x, yc - y, color);
	}
}

void mid_point_ellipse(Matrix *matrix, int32_t xc, int32_t yc, int32_t rx, int32_t ry, int32_t color, bool filled)
{
	int64_t a2 = rx * rx;
	int64_t b2 = ry * ry;
	int64_t twoa2 = 2 * a2;
	int64_t twob2 = 2 * b2;
	int64_t p;
	int32_t x = 0;
	int32_t y = ry;
	int64_t px = 0;
	int64_t py = twoa2 * y;

	/* Plot the initial point in each quadrant. */
	ellipse_plot_points(matrix, xc, yc, x, y, color, filled);

	/* Region 1 */
	p = (int64_t)round((double)b2 - (double)(a2 * ry) + (0.25 * (double)a2));
	while(px < py)
	{
		x++;
		px += twob2;
		if(p < 0)
		{
			p += b2 + px;
		}
		else
		{
			y--;
			py -= twoa2;
			p += b2 + px - py;
		}
		ellipse_plot_points(matrix, xc, yc, x, y, color, filled);
	}

	/* Region 2 */
	p = round(b2 * (x + 0.5) * (x + 0.5) + a2 * (y - 1) * (y - 1) - a2 * b2);
	while(y > 0)
	{
		y--;
		py -= twoa2;
		if(p > 0)
		{
			p += a2 - py;
		}
		else
		{
			x++;
			px += twob2;
			p += a2 - py + px;
		}
		ellipse_plot_points(matrix, xc, yc, x, y, color, filled);
	}
}

void draw_generic_ellipse(Matrix *matrix, int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t color, bool filled)
{
	int32_t max_dx = max(x1 - x0, y1 - y0);

	if(max_dx > 500)
	{
		mid_point_ellipse(matrix, x0 + (x1 - x0) / 2, y0 + (y1 - y0) / 2, (x1 - x0) / 2, (y1 - y0) / 2, color, filled);
	}
	else
	{
		brezenham_ellipse(matrix, x0, y0, x1, y1, color, filled);
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