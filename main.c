#include "defs.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define ASPECT_RATIO (16.0f / 9.0f)
#define IMAGE_HEIGHT 216
#define IMAGE_WIDTH (int) (IMAGE_HEIGHT * ASPECT_RATIO)

color compute_ray_color(struct ray *r);

int main()
{
        struct image *img = image_create(IMAGE_WIDTH, IMAGE_HEIGHT);
        struct camera *cam = camera_create(vec3(0.0f, 0.0f, 0.0f), 1.0f);

        for (int j = img->height - 1; j >= 0; j--) {
                fprintf(stderr, "\rscanlines remaining: %d    ", j);
                for (int i = 0; i < img->width; i++) {
                        struct ray r = ray_to_pixel(cam, img, i, j);
                        color c = compute_ray_color(&r);
                        image_set_pixel(img, i, j, c);
                }
        }
        fprintf(stderr, "\ndone.\n");

        image_write_ppm("out.ppm", img);

        camera_destroy(cam);
        image_destroy(img);

        return 0;
}

color compute_ray_color(struct ray *r)
{
        struct sphere sphere = {vec3(0.0f, 0.0f, -2.0f), 1.0f};
        color white = vec3(1.0f, 1.0f, 1.0f);
        color blue = vec3(0.5f, 0.7f, 1.0f);
        color ray_color;

	float t = ray_sphere_intersection(r, &sphere);
	if (t >= 0) {
		struct vec3 normal;
		struct vec3 hit_point = ray_at(r, t);
		vec3_sub(&normal, &hit_point, &sphere.origin);
		vec3_normalize(&normal, &normal);
		ray_color = vec3(normal.e[RED] + 1.0f,
				 normal.e[GREEN] + 1.0f,
				 normal.e[BLUE] + 1.0f);
		vec3_mult(&ray_color, &ray_color, 0.5f);
	} else {
		struct vec3 normalized;
		vec3_normalize(&normalized, &r->dir);

		float amt = normalized.e[Y_COOR];
		amt = norm(amt, -1.0f, 1.0f);

		vec3_mult(&blue, &blue, amt);
		vec3_mult(&white, &white, (1.0f - amt));
		vec3_add(&ray_color, &blue, &white);
	}

        return ray_color;
}
