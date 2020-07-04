#include "defs.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define ASPECT_RATIO (16.0f / 9.0f)
#define IMAGE_HEIGHT 216
#define IMAGE_WIDTH (int) (IMAGE_HEIGHT * ASPECT_RATIO)
#define NUM_SAMPLES 64

color compute_ray_color(struct ray *r, struct hittable_list *world);
struct hittable_list *build_world();

int main()
{
        struct image *img = image_create(IMAGE_WIDTH, IMAGE_HEIGHT);
        struct camera *cam = camera_create(vec3(0.0f, 0.0f, 1.0f), 1.0f);
        struct hittable_list *world = build_world();

        for (int j = img->height - 1; j >= 0; j--) {
                fprintf(stderr, "\rscanlines remaining: %d    ", j);
                for (int i = 0; i < img->width; i++) {
                        color accumulator = vec3(0.0f, 0.0f, 0.0f);
                        for (int k = 0; k < NUM_SAMPLES; k++) {
                                struct ray r = ray_to_pixel(cam, img, i, j);
                                struct vec3 tmp = compute_ray_color(&r, world);
                                vec3_add(&accumulator, &accumulator, &tmp);
                        }
                        vec3_div(&accumulator, &accumulator, NUM_SAMPLES);
                        image_set_pixel(img, i, j, accumulator);
                }
        }
        fprintf(stderr, "\ndone.\n");

        image_write_ppm("out.ppm", img);

        camera_destroy(cam);
        image_destroy(img);

        return 0;
}

struct hittable_list *build_world()
{
        struct hittable_list *world = hittable_list_create();
        struct sphere *obj1 = sphere_create(vec3(0.0f, 0.0f, -1.0f), 0.5f, vec3(0.9f, 0.3f, 0.3f));
        struct sphere *obj2 = sphere_create(vec3(1.0f, 0.0f, -1.0f), 0.5f, vec3(0.3f, 0.9f, 0.3f));
        struct sphere *obj3 = sphere_create(vec3(-1.0f, 0.0f, -1.0f), 0.5f, vec3(0.3f, 0.3f, 0.9f));
        struct sphere *obj4 = sphere_create(vec3(0.0f, -100.5f, -1.0f), 100.0f, vec3(0.8f, 0.8f, 0.8f));

        hittable_list_add(world, (struct hittable *) obj1);
        hittable_list_add(world, (struct hittable *) obj2);
        hittable_list_add(world, (struct hittable *) obj3);
        hittable_list_add(world, (struct hittable *) obj4);

        return world;
}

color evaluate_lambertian_model(struct directional_light *light, struct hit_record *hit)
{
        color pixel_color;
        float dot_product = vec3_dot(&hit->normal, &light->dir);

        dot_product = max(dot_product, 0);
        pixel_color.e[RED] = hit->surface_color.e[RED] * light->intensity.e[RED] * dot_product;
        pixel_color.e[GREEN] = hit->surface_color.e[GREEN] * light->intensity.e[GREEN] * dot_product;
        pixel_color.e[BLUE] = hit->surface_color.e[BLUE] * light->intensity.e[BLUE] * dot_product; 

        return pixel_color;
}

color compute_background_color(struct ray *r)
{
        color ray_color;
        color white = vec3(1.0f, 1.0f, 1.0f);
        color blue = vec3(0.5f, 0.7f, 1.0f);

        struct vec3 normalized;
        vec3_normalize(&normalized, &r->dir);

        float amt = normalized.e[Y_COOR];
        amt = norm(amt, -1.0f, 1.0f);

        vec3_mult(&blue, &blue, amt);
        vec3_mult(&white, &white, (1.0f - amt));
        vec3_add(&ray_color, &blue, &white);

        return ray_color;
}

color compute_ray_color(struct ray *r, struct hittable_list *world)
{
        color ray_color;

        struct hit_record hit;
        if (hittable_list_hit(world, r, 0.0f, INFINITY, &hit)) {
                struct directional_light light;
                light.dir = vec3(1.0f, 1.0f, 1.0f);
                vec3_normalize(&light.dir, &light.dir);
                light.intensity = vec3(1.0f, 1.0f, 1.0f);
                ray_color = evaluate_lambertian_model(&light, &hit);
        } else {
                ray_color = compute_background_color(r);
        }

        return ray_color;
}
