#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

typedef struct s_data{
	int w;
	int h;
	char c;
}t_data;

typedef struct s_shape{
	char c;
	char bg;
	float x;
	float y;
	float r;
}t_shape;

int main(int ac, char **av)
{
	FILE *file;
	t_data data;
	t_shape shape;
	char canvas[300][300];
	int x;
	int y;
	int exits;
	float dist;

	if (ac != 2)
	{
		write(1, "Error) arguments\n", 16);
		return (1);
	}
	printf("1\n");
	file = fopen(av[1], "r");
	if (!file)
	{
		write(1, "Error) \n", 32);
		return (1);
	}
	printf("2\n");
	if (fscanf(file, "%d %d %c\n", &data.w, &data.h, &data.c) != 3 || data.w <= 0 || data.w > 300 || data.h <= 0 || data.h > 300)
	{
		write(1, "Error) \n", 32);
		return (1);
	}
	printf("3\n");
	y = -1;
	while (++y < data.h)
	{
		x = -1;
		while (++x < data.w)
			canvas[y][x] = data.c;
	}
	printf("4\n");
	while ((exits = fscanf(file, "%c %f %f %f %c\n", &shape.c, &shape.x, &shape.y, &shape.r, &shape.bg)) == 5)
	{
		if (shape.r <= 0 || (shape.c != 'c' && shape.c != 'C'))
		{
			write(1, "Error) \n", 32);
			return (1);
		}
		y = -1;
		while (++y < data.h)
		{
			x = -1;
			while (++x < data.w)
			{
				dist = sqrtf(powf(x - shape.x, 2) + powf(y - shape.y, 2));
				if (dist <= shape.r)
				{
					if (dist <= shape.r && dist > shape.r - 1)
						canvas[y][x] = shape.bg;
					else if (shape.c == 'C')
						canvas[y][x] = shape.bg;
				}
			}
		}
	}
	if (exits != -1 && exits)
	{
		write(1, "Error) ", 32);
		return (1);
	}
	y = -1;
	while (++y < data.h)
	{
		x = -1;
		while (++x < data.w)
		{
			write(1, &canvas[y][x], 1);
			if (x == data.w - 1)
				write(1, "\n", 1);
		}
	}
	fclose(file);
	return (0);
}