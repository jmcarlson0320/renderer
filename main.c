#include "defs.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define ASPECT_RATIO (16.0f / 9.0f)
#define IMAGE_HEIGHT 216
#define IMAGE_WIDTH (int) (216 * ASPECT_RATIO)

int main()
{
	struct image *img = image_create(IMAGE_WIDTH, IMAGE_HEIGHT);
        struct camera *cam = camera_create(vec3(0.0f, 0.0f, 0.0f), 1.0f);

	for (int j = img->height - 1; j >= 0; j--) {
		fprintf(stderr, "\rscanlines remaining: %d    ", j);
		for (int i = 0; i < img->width; i++) {
			color c = vec3(0, 0.5, 0);
			image_set_pixel(img, i, j, c);
		}
	}
	fprintf(stderr, "\ndone.\n");

	image_write_ppm("out.ppm", img);

        camera_destroy(cam);
        image_destroy(img);

	return 0;
}
