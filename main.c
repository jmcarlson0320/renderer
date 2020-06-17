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
        color white = vec3(1.0f, 1.0f, 1.0f);
        color blue = vec3(0.1f, 0.5f, 0.7f);
        color ray_color;

        struct vec3 normalized;
        vec3_normalize(&normalized, &r->dir);

        float t = normalized.e[Y_COOR];
        t = norm(t, -1.0f, 1.0f);

        vec3_mult(&blue, &blue, t);
        vec3_mult(&white, &white, (1.0f - t));
        vec3_add(&ray_color, &blue, &white);

        return ray_color;
}
