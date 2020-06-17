#include "defs.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main()
{
	float aspect_ratio = 16.0f / 9.0f;
	struct image img;
	img.width = 384;
	img.height = (int) img.width / aspect_ratio;
	img.pixels = malloc(img.width * img.height * sizeof(color));

	for (int j = img.height - 1; j >= 0; j--) {
		fprintf(stderr, "\rscanlines remaining: %d    ", j);
		for (int i = 0; i < img.width; i++) {
			color c = vec3(0, 0, 0);
			image_set_pixel(&img, i, j, c);
		}
	}
	fprintf(stderr, "\ndone.\n");

	image_write_ppm("out.ppm", &img);

	free(img.pixels);

	return 0;
}
