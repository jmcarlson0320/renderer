#include "defs.h"
#include <stdio.h>
#include <stdlib.h>

struct image *image_create(int width, int height)
{
	struct image *img = malloc(sizeof(struct image));
	img->width = width;
	img->height = height;
	img->pixels = malloc(width * height * sizeof(color));
	return img;
}

void image_destroy(struct image *img)
{
	free(img->pixels);
	free(img);
}

int image_set_pixel(struct image *img, int x, int y, color c)
{
	if (x < 0 || img->width <= x || y < 0 || img->height <= y)
		return ERROR;

	img->pixels[y * img->width + x] = c;

	return 0;
}

void image_write_ppm(char *filename, const struct image *img)
{
	FILE *fs = fopen(filename, "w");
	if (!fs) {
		fprintf(stderr, "could not open file %s\n", filename);
		return;
	}

	fprintf(fs, "P3\n%d %d\n255\n", img->width, img->height);
	for (int j = img->height - 1; j >= 0; j--) {
		for (int i = 0; i < img->width; i++) {
			color c = img->pixels[j * img->width + i];
			write_color(fs, c);
		}
	}
}
