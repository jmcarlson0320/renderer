#include "defs.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define ASPECT_RATIO (16.0f / 9.0f)
#define IMAGE_HEIGHT 216
#define IMAGE_WIDTH (int) (IMAGE_HEIGHT * ASPECT_RATIO)
#define ANTI_ALIASING TRUE
#define AA_ON 1
#define AA_OFF 0
#define NUM_SAMPLES 100

color compute_ray_color(struct ray *r, struct hittable_list *world);

int main()
{
        struct image *img = image_create(IMAGE_WIDTH, IMAGE_HEIGHT);
        struct camera *cam = camera_create(vec3(0.0f, 0.0f, 0.0f), 1.0f);
        struct hittable_list *world = hittable_list_create();
        struct sphere *obj1 = sphere_create(vec3(0.0f, 0.0f, -1.0f), 0.5f);
        struct sphere *obj2 = sphere_create(vec3(0.0f, -100.5f, -1.0f), 100.0f);
        hittable_list_add(world, (struct hittable *) obj1);
        hittable_list_add(world, (struct hittable *) obj2);

        for (int j = img->height - 1; j >= 0; j--) {
                fprintf(stderr, "\rscanlines remaining: %d    ", j);
                for (int i = 0; i < img->width; i++) {
                        color accumulator = vec3(0.0f, 0.0f, 0.0f);
                        if (ANTI_ALIASING) {
                                for (int k = 0; k < NUM_SAMPLES; k++) {
                                        struct ray r = ray_to_pixel(cam, img, i, j, AA_ON);
                                        color c = compute_ray_color(&r, world);
                                        vec3_add(&accumulator, &accumulator, &c);
                                }
                                vec3_div(&accumulator, &accumulator, NUM_SAMPLES);
                        } else {
                                struct ray r = ray_to_pixel(cam, img, i, j, AA_OFF);
                                accumulator = compute_ray_color(&r, world);
                        }
                        image_set_pixel(img, i, j, accumulator);
                }
        }
        fprintf(stderr, "\ndone.\n");

        image_write_ppm("out.ppm", img);

        sphere_destroy(obj1);
        sphere_destroy(obj2);
        hittable_list_destroy(world);
        camera_destroy(cam);
        image_destroy(img);

        return 0;
}

color compute_ray_color(struct ray *r, struct hittable_list *world)
{
        color ray_color;
        struct hit_record hit;

        if (hittable_list_hit(world, r, 0.001f, INFINITY, &hit)) {
                struct vec3 target;
                struct vec3 random_point = rand_in_unit_sphere();
                vec3_add(&target, &hit.hit_point, &hit.normal);
                vec3_add(&target, &target, &random_point);
                struct ray new_ray = {hit.hit_point, target};
                ray_color = compute_ray_color(&new_ray, world);
                vec3_mult(&ray_color, &ray_color, 0.5f);
                return ray_color;
        } else {
                color white = vec3(1.0f, 1.0f, 1.0f);
                color blue = vec3(0.5f, 0.7f, 1.0f);

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
