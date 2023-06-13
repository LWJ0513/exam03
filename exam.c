#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <math.h>

typedef struct map
{
	int width;
	int height;
	char c;
	char map[300][300];
} t_canvus;

typedef struct a
{
	char type;
	float x;
	float y;
	float radius;
	char c;
} t_won;

typedef struct b
{
	char type;
	float x;
	float y;
	float garo;
	float sero;
	char c;
} t_nemo;

int ft_strlen(char *s)
{
	int i = 0;

	while (s[i])
		i++;
	return i;
}

int err(char *s)
{
	write(1, s, ft_strlen(s));
	return 1;
}

// -- 원일 때
int won(int argc, char **argv)
{
	t_canvus canvus;
	t_won won;

	memset(&canvus, 0, sizeof(t_canvus));
	memset(&won, 0, sizeof(t_won));

	if (argc != 2)
		return err("Error: argument\n");

	FILE *f = fopen(argv[1], "r");
	if (!f)
		return err("Error: Operation file corrupted\n");

	if (fscanf(f, "%d %d %c\n", &canvus.width, &canvus.height, &canvus.c) != 3)
	{
		fclose(f);
		return err("Error: Operation file corrupted\n");
	}

	if (canvus.width <= 0 || canvus.height <= 0 || canvus.width > 300 || canvus.height > 300)
	{
		fclose(f);
		return err("Error: Operation file corrupted\n");
	}

	for (int x = 0; x < canvus.height; x++)
	{
		for (int y = 0; y < canvus.width; y++)
			canvus.map[x][y] = canvus.c;
	}

	int r;
	while ((r = fscanf(f, " %c %f %f %f %c", &won.type, &won.x, &won.y, &won.radius, &won.c)) == 5)
	{
		if (won.type != 'C' && won.type != 'c')
		{
			fclose(f);
			return err("Error: Operation file corrupted\n");
		}
		if (won.radius <= 0.0f)
		{
			fclose(f);
			return err("Error: Operation file corrupted\n");
		}

		for (int y = 0; y < canvus.height; y++)
		{
			for (int x = 0; x < canvus.width; x++)
			{
				float g = sqrtf(powf(x - won.x, 2) + powf(y - won.y, 2));

				if (g <= won.radius)
				{
					if (won.type == 'C')
						canvus.map[y][x] = won.c;
					if (won.type == 'c' && g - won.radius < 1 && g - won.radius > -1)
						canvus.map[y][x] = won.c;
				}
			}
		}
		memset(&won, 0, sizeof(t_won));
	}

	if (r != 5 && r != EOF)
	{
		fclose(f);
		return err("Error: Operation file corrupted\n");
	}

	fclose(f);

	for (int x = 0; x < canvus.height; x++)
	{
		for (int y = 0; y < canvus.width; y++)
			write(1, &canvus.map[x][y], 1);
		write(1, "\n", 1);
	}
	return 0;
}

// -- 네모일ㄷ 때
int nemo(int argc, char **argv)
{
	t_canvus canvus;
	t_nemo nemo;

	memset(&canvus, 0, sizeof(t_canvus));
	memset(&nemo, 0, sizeof(t_nemo));

	if (argc != 2)
		return err("Error: argument\n");

	FILE *f = fopen(argv[1], "r");
	if (!f)
		return err("Error: Operation file corrupted\n");

	if (fscanf(f, "%d %d %c\n", &canvus.width, &canvus.height, &canvus.c) != 3)
	{
		fclose(f);
		return err("Error: Operation file corrupted\n");
	}

	if (canvus.width <= 0 || canvus.height <= 0 || canvus.width > 300 || canvus.height > 300)
	{
		fclose(f);
		return err("Error: Operation file corrupted\n");
	}

	for (int x = 0; x < canvus.height; x++)
	{
		for (int y = 0; y < canvus.width; y++)
			canvus.map[x][y] = canvus.c;
	}

	int r;
	while ((r = fscanf(f, " %c %f %f %f %f %c", &nemo.type, &nemo.x, &nemo.y, &nemo.garo, &nemo.sero, &nemo.c)) == 6)
	{
		if (nemo.type != 'R' && nemo.type != 'r')
		{
			fclose(f);
			return err("Error: Operation file corrupted\n");
		}
		if (nemo.garo <= 0 || nemo.sero <= 0)
		{
			fclose(f);
			return err("Error: Operation file corrupted\n");
		}

		for (int y = 0; y < canvus.height; y++)
		{
			for (int x = 0; x < canvus.width; x++)
			{
				if (x >= nemo.x && x <= nemo.x + nemo.garo && y >= nemo.y && y <= nemo.y + nemo.sero)
				{
					if (nemo.type == 'R')
						canvus.map[y][x] = nemo.c;

					// ???
					if (nemo.type == 'r' && (x - nemo.x < 1.0f || y - nemo.y < 1.0f || nemo.x + nemo.garo - x < 1.0f || nemo.y + nemo.sero - y < 1.0f))
						canvus.map[y][x] = nemo.c;
				}
			}
		}
		memset(&nemo, 0, sizeof(t_nemo));
	}

	if (r != 6 && r != EOF)
	{
		fclose(f);
		return err("Error: Operation file corrupted\n");
	}

	fclose(f);

	for (int x = 0; x < canvus.height; x++)
	{
		for (int y = 0; y < canvus.width; y++)
			write(1, &canvus.map[x][y], 1);
		write(1, "\n", 1);
	}
	return 0;
}

int main(int argc, char **argv)
{
	// return won(argc, argv);
	return nemo(argc, argv);
}
