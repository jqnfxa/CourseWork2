#include "polygon.h"
#include "../../Validator/validator.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>

/*
 * private attributes
 */
#define round_up(f) ((int)((double)(f) >= 0.0 ? floor((double)(f) + 0.5) : -floor(fabs((double)f) + 0.5)))
#define round_down(f) ((int)((double)(f) >= 0.0 ? ceil((double)(f) - 0.5) : -ceil(fabs((double)f) - 0.5)))

typedef struct {
	/* edge descriptor for polygon engine */
	int d;
	int x0, y0;
	int xmin, ymin, xmax, ymax;
	float dx;
} Edge;

int draw_wide_line(Matrix *matrix, int x0, int y0, int x1, int y1, int color, int width, int danger_color);
void draw_elementary_line(Matrix *matrix, int x0, int y0, int x1, int y1, int color, int danger_color);
void hline(Matrix *matrix, int x0, int y0, int x1, int color, int danger_color);
int x_cmp(const void *x0, const void *x1);
int polygon_generic(Matrix *matrix, int n, Edge *e, int color, int danger_color);
int draw_elementary_polygon(Matrix *matrix, int count, int *xy, int color, int fill, int width, int danger_color);
// end of private

/*
 * public methods
 */
void draw_line(Matrix *matrix, LineQuery *info)
{
	if(matrix == NULL || info == NULL)
	{
		return;
	}

	if(info->width == 1)
	{
		draw_elementary_line(matrix, info->start.x, info->start.y, info->end.x, info->end.y, info->color, -1);
	}
	else
	{
		draw_wide_line(matrix, info->start.x, info->start.y, info->end.x, info->end.y, info->color, info->width, -1);
	}
}

void draw_polygon(Matrix *matrix, PolygonQuery *info)
{
	if(matrix == NULL || info == NULL)
	{
		return;
	}
	if(match_flags(info->check_sum, FILL))
	{
		draw_elementary_polygon(matrix, info->points_count, info->points, info->fill_color, true, 1, -1);
	}
	if(info->color != info->fill_color)
	{
		if(info->width == 1)
		{
			draw_elementary_polygon(matrix, info->points_count, info->points, info->color, 0, 1, -1);
		}
		else
		{
			Matrix new_image = create(matrix->height, matrix->width);

			int avoid_color = invert_color(info->color);
			draw_elementary_polygon(&new_image, info->points_count, info->points, avoid_color, 1, 1, -1);

			int width = info->width * 2 - 1;

			draw_elementary_polygon(&new_image, info->points_count, info->points, info->color, 0, width, 0x0);

			int avoid[] = {avoid_color, 0x0};

			copy_additional(matrix, &new_image, avoid, 2);

			destroy(&new_image);
		}
	}
}

void draw_elementary_line(Matrix *matrix, int x0, int y0, int x1, int y1, int color, int danger_color)
{
	int i, n, e;
	int dx, dy;
	int xs, ys;

	/* normalize coordinates */
	dx = x1 - x0;
	if(dx < 0)
	{
		dx = -dx, xs = -1;
	}
	else
	{
		xs = 1;
	}
	dy = y1 - y0;
	if(dy < 0)
	{
		dy = -dy, ys = -1;
	}
	else
	{
		ys = 1;
	}

	n = (dx > dy) ? dx : dy;

	if(dx == 0)
	{
		/* vertical */
		for(i = 0; i < dy; i++)
		{
			if(danger_color == -1)
			{
				set_pixel(matrix, x0, y0, color);
			}
			else
			{
				set_pixel_avoid(matrix, x0, y0, color, danger_color);
			}
			y0 += ys;
		}
	}
	else if(dy == 0)
	{
		/* horizontal */
		for(i = 0; i < dx; i++)
		{
			if(danger_color == -1)
			{
				set_pixel(matrix, x0, y0, color);
			}
			else
			{
				set_pixel_avoid(matrix, x0, y0, color, danger_color);
			}
			x0 += xs;
		}
	}
	else if(dx > dy)
	{
		/* bresenham, horizontal slope */
		n = dx;
		dy += dy;
		e = dy - dx;
		dx += dx;

		for(i = 0; i < n; i++)
		{
			if(danger_color == -1)
			{
				set_pixel(matrix, x0, y0, color);
			}
			else
			{
				set_pixel_avoid(matrix, x0, y0, color, danger_color);
			}
			if(e >= 0)
			{
				y0 += ys;
				e -= dx;
			}
			e += dy;
			x0 += xs;
		}
	}
	else
	{
		/* bresenham, vertical slope */
		n = dy;
		dx += dx;
		e = dx - dy;
		dy += dy;

		for(i = 0; i < n; i++)
		{
			if(danger_color == -1)
			{
				set_pixel(matrix, x0, y0, color);
			}
			else
			{
				set_pixel_avoid(matrix, x0, y0, color, danger_color);
			}
			if(e >= 0)
			{
				x0 += xs;
				e -= dy;
			}
			e += dx;
			y0 += ys;
		}
	}
}

void hline(Matrix *matrix, int x0, int y0, int x1, int color, int danger_color)
{
	if(y0 >= 0 && y0 < matrix->height)
	{
		if(x0 < 0)
		{
			x0 = 0;
		}
		else if(x0 >= matrix->width)
		{
			return;
		}
		if(x1 < 0)
		{
			return;
		}
		else if(x1 >= matrix->width)
		{
			x1 = matrix->width - 1;
		}
		while(x0 <= x1)
		{
			set_pixel_avoid(matrix, x0, y0, color, danger_color);
			++x0;
		}
	}
}

int x_cmp(const void *x0, const void *x1)
{
	float diff = *((float *)x0) - *((float *)x1);
	if(diff < 0)
	{
		return -1;
	}
	else if(diff > 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int polygon_generic(Matrix *matrix, int n, Edge *e, int color, int danger_color)
{
	Edge **edge_table;
	float *xx;
	int edge_count = 0;
	int y_min = matrix->height - 1;
	int y_max = 0;
	int i, j, k;
	float adjacent_line_x, adjacent_line_x_other_edge;

	if(n <= 0)
	{
		return 0;
	}

	/* Initialize the edge table and find polygon boundaries */
	/* malloc check ok, using calloc */
	edge_table = calloc(n, sizeof(Edge *));
	if(!edge_table)
	{
		return -1;
	}

	for(i = 0; i < n; i++)
	{
		if(y_min > e[i].ymin)
		{
			y_min = e[i].ymin;
		}
		if(y_max < e[i].ymax)
		{
			y_max = e[i].ymax;
		}
		if(e[i].ymin == e[i].ymax)
		{
			hline(matrix, e[i].xmin, e[i].ymin, e[i].xmax, color, danger_color);
			continue;
		}
		edge_table[edge_count++] = (e + i);
	}
	if(y_min < 0)
	{
		y_min = 0;
	}
	if(y_max > matrix->height)
	{
		y_max = matrix->height;
	}

	/* Process the edge table with a scan line searching for intersections */
	xx = calloc(edge_count * 2, sizeof(float));
	if(!xx)
	{
		free(edge_table);
		return -1;
	}
	for(; y_min <= y_max; y_min++)
	{
		j = 0;
		for(i = 0; i < edge_count; i++)
		{
			Edge *current = edge_table[i];
			if(y_min >= current->ymin && y_min <= current->ymax)
			{
				xx[j++] = (float)(y_min - current->y0) * current->dx + (float)current->x0;

				if(y_min == current->ymax && y_min < y_max)
				{
					// Needed to draw consistent polygons
					xx[j] = xx[j - 1];
					j++;
				}
				else if(current->dx != 0 && roundf(xx[j - 1]) == xx[j - 1])
				{
					// Connect discontiguous corners
					for(k = 0; k < i; k++)
					{
						Edge *other_edge = edge_table[k];
						if((current->dx > 0 && other_edge->dx <= 0) || (current->dx < 0 && other_edge->dx >= 0))
						{
							continue;
						}
						// Check if the two edges join to make a corner
						if(((y_min == current->ymin && y_min == other_edge->ymin) ||
							(y_min == current->ymax && y_min == other_edge->ymax)) &&
						   xx[j - 1] == (float)(y_min - other_edge->y0) * other_edge->dx + (float)other_edge->x0)
						{
							// Determine points from the edges on the next row
							// Or if this is the last row, check the previous row
							int offset = y_min == y_max ? -1 : 1;
							adjacent_line_x = (float)(y_min + offset - current->y0) * current->dx +
											  (float)current->x0;
							adjacent_line_x_other_edge = (float)(y_min + offset - other_edge->y0) * other_edge->dx +
														 (float)other_edge->x0;
							if(y_min == current->ymax)
							{
								if(current->dx > 0)
								{
									xx[k] = fmaxf(adjacent_line_x, adjacent_line_x_other_edge) + 1;
								}
								else
								{
									xx[k] = fminf(adjacent_line_x, adjacent_line_x_other_edge) - 1;
								}
							}
							else
							{
								if(current->dx > 0)
								{
									xx[k] = fminf(adjacent_line_x, adjacent_line_x_other_edge);
								}
								else
								{
									xx[k] = fmaxf(adjacent_line_x, adjacent_line_x_other_edge) + 1;
								}
							}
							break;
						}
					}
				}
			}
		}
		qsort(xx, j, sizeof(float), x_cmp);

		for(i = 1; i < j; i += 2)
		{
			hline(matrix, round_up(xx[i - 1]), y_min, round_down(xx[i]), color, danger_color);
		}
	}

	free(xx);
	free(edge_table);
	return 0;
}

void add_edge(Edge *e, int x0, int y0, int x1, int y1)
{
	if(x0 <= x1)
	{
		e->xmin = x0, e->xmax = x1;
	}
	else
	{
		e->xmin = x1, e->xmax = x0;
	}

	if(y0 <= y1)
	{
		e->ymin = y0, e->ymax = y1;
	}
	else
	{
		e->ymin = y1, e->ymax = y0;
	}

	if(y0 == y1)
	{
		e->d = 0;
		e->dx = 0.0f;
	}
	else
	{
		e->dx = ((float)(x1 - x0)) / ((float)(y1 - y0));
		if(y0 == e->ymin)
		{
			e->d = 1;
		}
		else
		{
			e->d = -1;
		}
	}

	e->x0 = x0;
	e->y0 = y0;
}

int draw_wide_line(Matrix *matrix, int x0, int y0, int x1, int y1, int color, int width, int danger_color)
{
	int dx, dy;
	double big_hypotenuse, small_hypotenuse, ratio_max, ratio_min;
	int dx_min, dx_max, dy_min, dy_max;
	Edge e[4];

	dx = x1 - x0;
	dy = y1 - y0;
	if(dx == 0 && dy == 0)
	{
		set_pixel_avoid(matrix, x0, y0, color, danger_color);
		return 0;
	}

	big_hypotenuse = hypot(dx, dy);
	small_hypotenuse = (width - 1) / 2.0;
	ratio_max = round_up(small_hypotenuse) / big_hypotenuse;
	ratio_min = round_down(small_hypotenuse) / big_hypotenuse;

	dx_min = round_down(ratio_min * dy);
	dx_max = round_down(ratio_max * dy);
	dy_min = round_down(ratio_min * dx);
	dy_max = round_down(ratio_max * dx);
	{
		int vertices[4][2] = {
			{x0 - dx_min, y0 + dy_max},
			{x1 - dx_min, y1 + dy_max},
			{x1 + dx_max, y1 - dy_min},
			{x0 + dx_max, y0 - dy_min}};

		add_edge(e + 0, vertices[0][0], vertices[0][1], vertices[1][0], vertices[1][1]);
		add_edge(e + 1, vertices[1][0], vertices[1][1], vertices[2][0], vertices[2][1]);
		add_edge(e + 2, vertices[2][0], vertices[2][1], vertices[3][0], vertices[3][1]);
		add_edge(e + 3, vertices[3][0], vertices[3][1], vertices[0][0], vertices[0][1]);

		polygon_generic(matrix, 4, e, color, danger_color);
	}
	return 0;
}

int draw_elementary_polygon(Matrix *matrix, int count, int *xy, int color, int fill, int width, int danger_color)
{
	int i, n, x0, y0, x1, y1;

	if(count <= 0)
	{
		return 0;
	}

	if(fill)
	{
		/* Build edge list */
		/* malloc check ok, using calloc */
		Edge *e = calloc(count, sizeof(Edge));
		if(!e)
		{
			return -1;
		}
		for(i = n = 0; i < count - 1; i++)
		{
			x0 = xy[i * 2];
			y0 = xy[i * 2 + 1];
			x1 = xy[i * 2 + 2];
			y1 = xy[i * 2 + 3];
			if(y0 == y1 && i != 0 && y0 == xy[i * 2 - 1])
			{
				// This is a horizontal line,
				// that immediately follows another horizontal line
				Edge *last_e = &e[n - 1];
				if(x1 > x0 && x0 > xy[i * 2 - 2])
				{
					// They are both increasing in x
					last_e->xmax = x1;
					continue;
				}
				else if(x1 < x0 && x0 < xy[i * 2 - 2])
				{
					// They are both decreasing in x
					last_e->xmin = x1;
					continue;
				}
			}
			add_edge(&e[n++], x0, y0, x1, y1);
		}
		if(xy[i * 2] != xy[0] || xy[i * 2 + 1] != xy[1])
		{
			add_edge(&e[n++], xy[i * 2], xy[i * 2 + 1], xy[0], xy[1]);
		}
		polygon_generic(matrix, n, e, color, danger_color);
		free(e);
	}
	else
	{
		/* Outline */
		if(width == 1)
		{
			for(i = 0; i < count - 1; i++)
			{
				draw_elementary_line(matrix, xy[i * 2], xy[i * 2 + 1], xy[i * 2 + 2], xy[i * 2 + 3], color, danger_color);
			}
			draw_elementary_line(matrix, xy[i * 2], xy[i * 2 + 1], xy[0], xy[1], color, danger_color);
		}
		else
		{
			for(i = 0; i < count - 1; i++)
			{
				draw_wide_line(matrix, xy[i * 2], xy[i * 2 + 1], xy[i * 2 + 2], xy[i * 2 + 3], color, width, danger_color);
			}
			draw_wide_line(matrix, xy[i * 2], xy[i * 2 + 1], xy[0], xy[1], color, width, danger_color);
		}
	}

	return 0;
}