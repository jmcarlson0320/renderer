#include "defs.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void renderer_init(struct renderer *renderer, void *pixels)
{
        renderer->image = malloc(sizeof(struct image));
        renderer->image->width = IMAGE_WIDTH;
        renderer->image->height = IMAGE_HEIGHT;
        renderer->image->pixels = (color *) pixels;

        renderer->camera = camera_create(vec3(0.0f, 0.0f, 1.0f), 1.0f);
        renderer->world = hittable_list_create();

        struct sphere *obj1 = sphere_create(vec3(0.0f, 0.0f, -1.0f), 0.5f,
                                            vec3(0.9f, 0.3f, 0.3f));
        struct sphere *obj2 = sphere_create(vec3(1.0f, 0.0f, -1.0f), 0.5f,
                                            vec3(0.3f, 0.9f, 0.3f));
        struct sphere *obj3 = sphere_create(vec3(-1.0f, 0.0f, -1.0f), 0.5f,
                                            vec3(0.3f, 0.3f, 0.9f));
        struct sphere *obj4 = sphere_create(vec3(0.0f, -100.5f, -1.0f), 100.0f,
                                            vec3(0.8f, 0.8f, 0.8f));
        hittable_list_add(renderer->world, (struct hittable *) obj1);
        hittable_list_add(renderer->world, (struct hittable *) obj2);
        hittable_list_add(renderer->world, (struct hittable *) obj3);
        hittable_list_add(renderer->world, (struct hittable *) obj4);
}

void renderer_update(struct renderer *renderer)
{
        int width = renderer->image->width;
        int height = renderer->image->height;
        for (int j = height - 1; j >= 0; j--) {
                for (int i = 0; i < width; i++) {
                        color accumulator = vec3(0.0f, 0.0f, 0.0f);
                        struct ray r = ray_to_pixel(renderer->camera, renderer->image, i, j);
                        accumulator = compute_ray_color(&r, renderer->world);
                        image_set_pixel(renderer->image, i, j, accumulator);
                }
        }

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
                ray_color.e[RED] = hit.surface_color.e[RED]
                                 * light_intensity
                                 * dot_product;
                ray_color.e[GREEN] = hit.surface_color.e[GREEN]
                                   * light_intensity
                                   * dot_product;
                ray_color.e[BLUE] = hit.surface_color.e[BLUE]
                                  * light_intensity
                                  * dot_product;
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
