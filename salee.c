#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct draw_zone
{
	int width;
	int height;
	char *zone;
} Dzone;

typedef struct rectangle
{
	char type;
	float x;
	float y;
	float width;
	float height;
	char color;
} Rect;

int get_str_len(char *str)
{
	int len = 0;

	while (str[len])
		++len;
	return len;
}

int read_first_line(FILE *fp, Dzone *dzone)
{
	char back;

	if (fscanf(fp, "%d %d %c\n", &dzone->width, &dzone->height, &back) != 3)
		return 1;
	if (dzone->width < 1 || dzone->width > 300 || dzone->height < 1 || dzone->height > 300)
		return 1;
	int zone_size = dzone->width * dzone->height;
	dzone->zone = (char *)malloc(zone_size);
	if (dzone->zone == NULL)
		return 1;
	for (int x = 0; x < zone_size; ++x)
		dzone->zone[x] = back;
	return 0;
}

int is_in_rect(float x, float y, Rect *rect)
{
	if (x < rect->x || rect->x + rect->width < x || y < rect->y || rect->y + rect->height < y)
		return 0;
	if (x < rect->x + 1.00000000 || rect->x + rect->width < x + 1.00000000 ||
		y < rect->y + 1.00000000 || rect->y + rect->height < y + 1.00000000)
		return 2; // boundary
	return 1;	  // inside
}

void one_op(Rect *rect, Dzone *dzone, int x, int y)
{
	int is_in = is_in_rect(x, y, rect);

	if (is_in == 2 || (is_in == 1 && (rect->type == 'R')))
		dzone->zone[x + y * dzone->width] = rect->color;
	return;
}

int do_op(Rect *rect, Dzone *dzone)
{
	if (rect->width <= 0.00000000 || rect->height <= 0.00000000)
		return 1;
	if (rect->type != 'R' && rect->type != 'r')
		return 1;
	for (int y = 0; y < dzone->height; ++y)
	{
		for (int x = 0; x < dzone->width; ++x)
		{
			one_op(rect, dzone, x, y);
		}
	}
	return 0;
}

void print_zone(Dzone *dzone)
{
	for (int y = 0; y < dzone->height; ++y)
	{
		for (int x = 0; x < dzone->width; ++x)
		{
			char c = dzone->zone[x + y * dzone->width];
			write(1, &c, 1);
		}
		write(1, "\n", 1);
	}
	return;
}

int paint(FILE *fp)
{
	Dzone dzone;
	Rect rect;

	if (read_first_line(fp, &dzone) == 1)
		return 1;
	int ret = fscanf(fp, "%c %f %f %f %f %c\n", &rect.type, &rect.x, &rect.y, &rect.width, &rect.height, &rect.color);
	while (ret == 6)
	{
		if (do_op(&rect, &dzone))
			return 1;
		ret = fscanf(fp, "%c %f %f %f %f %c\n", &rect.type, &rect.x, &rect.y, &rect.width, &rect.height, &rect.color);
	}
	if (ret == -1)
	{
		print_zone(&dzone);
		return 0;
	}
	return 1;
}

int main(int argc, char **argv)
{

	if (argc != 2)
	{
		char *emsg = "Error: argument\n";
		write(1, emsg, get_str_len(emsg));
		return 1;
	}

	FILE *fp = fopen(argv[1], "r");
	if (!fp)
	{
		char *emsg = "Error: Operation file corrupted\n";
		write(1, emsg, get_str_len(emsg));
		return 1;
	}
	if (paint(fp) == 0)
		return 0;
	else
		return 1;
}
