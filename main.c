#include "defs.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define ASPECT_RATIO (16.0f / 9.0f)
#define IMAGE_HEIGHT 432
#define IMAGE_WIDTH (int) (IMAGE_HEIGHT * ASPECT_RATIO)

color compute_ray_color(struct ray *r, struct hittable_list *world);

int main()
{
        struct image *img = image_create(IMAGE_WIDTH, IMAGE_HEIGHT);
        struct camera *cam = camera_create(vec3(0.0f, 0.0f, 1.0f), 1.0f);
        struct hittable_list *world = hittable_list_create();

        struct sphere *obj1 = sphere_create(vec3(0.0f, 0.0f, -1.0f), 0.5f, vec3(0.9f, 0.3f, 0.3f));
        struct sphere *obj2 = sphere_create(vec3(1.0f, 0.0f, -1.0f), 0.5f, vec3(0.3f, 0.9f, 0.3f));
        struct sphere *obj3 = sphere_create(vec3(-1.0f, 0.0f, -1.0f), 0.5f, vec3(0.3f, 0.3f, 0.9f));
        struct sphere *obj4 = sphere_create(vec3(0.0f, -100.5f, -1.0f), 100.0f, vec3(0.8f, 0.8f, 0.8f));
        hittable_list_add(world, (struct hittable *) obj1);
        hittable_list_add(world, (struct hittable *) obj2);
        hittable_list_add(world, (struct hittable *) obj3);
        hittable_list_add(world, (struct hittable *) obj4);

        for (int j = img->height - 1; j >= 0; j--) {
                fprintf(stderr, "\rscanlines remaining: %d    ", j);
                for (int i = 0; i < img->width; i++) {
                        color accumulator = vec3(0.0f, 0.0f, 0.0f);
                        struct ray r = ray_to_pixel(cam, img, i, j);
                        accumulator = compute_ray_color(&r, world);
                        image_set_pixel(img, i, j, accumulator);
                }
        }
        fprintf(stderr, "\ndone.\n");

        image_write_ppm("out.ppm", img);

        sphere_destroy(obj1);
        sphere_destroy(obj2);
        sphere_destroy(obj3);
        sphere_destroy(obj4);
        hittable_list_destroy(world);
        camera_destroy(cam);
        image_destroy(img);

        return 0;
}

color compute_ray_color(struct ray *r, struct hittable_list *world)
{
        color white = vec3(1.0f, 1.0f, 1.0f);
        color blue = vec3(0.5f, 0.7f, 1.0f);
        color ray_color;

        struct vec3 light_dir = vec3(-1.0f, 1.0f, 1.0f);
        vec3_normalize(&light_dir, &light_dir);
        float light_intensity = 1.0f;

        struct hit_record hit;
        if (hittable_list_hit(world, r, 0.0f, INFINITY, &hit)) {
                float dot_product = vec3_dot(&hit.normal, &light_dir);
                dot_product = max(dot_product, 0);
                ray_color.e[RED] = hit.surface_color.e[RED] * light_intensity * dot_product;
                ray_color.e[GREEN] = hit.surface_color.e[GREEN] * light_intensity * dot_product;
                ray_color.e[BLUE] = hit.surface_color.e[BLUE] * light_intensity * dot_product;
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
