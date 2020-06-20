#include "defs.h"
#include <stdlib.h>
#include <math.h>

int sphere_hit(const struct hittable *hittable, const struct ray *ray,
               float t_0, float t_1,
               struct hit_record *record)
{
        float a;
        float b;
        float c;
        float discriminant;

        struct sphere *sphere = (struct sphere *) hittable;

        struct vec3 tmp;
        vec3_sub(&tmp, &ray->origin, &sphere->origin);

        a = vec3_len_sqr(&ray->dir);
        b = (2.0f) * vec3_dot(&ray->dir, &tmp);
        c = vec3_len_sqr(&tmp) - (sphere->radius * sphere->radius);

        discriminant = b * b - 4.0f * a * c;

        if (discriminant < 0)
                return FALSE;

        float t = (-b - sqrt(discriminant)) / (2.0f * a);
        if (!in_range_inclusive(t, t_0, t_1))
                return FALSE;

        record->t = t;
        record->hit_point = ray_at(ray, t);
        vec3_sub(&record->normal, &record->hit_point, &sphere->origin);
        vec3_normalize(&record->normal, &record->normal);
        record->surface_color = sphere->surface_color;

        if (vec3_dot(&record->normal, &ray->dir) > 0) {
                vec3_mult(&record->normal, &record->normal, -1.0f);
                record->front_face = FALSE;
        } else {
                record->front_face = TRUE;
        }

        return TRUE;
}

static const struct vtable sphere_vtable = {
        sphere_hit
};

struct sphere *sphere_create(struct vec3 pos, float radius,
                             color surface_color)
{
        struct sphere *sphere = malloc(sizeof(struct sphere));
        sphere->hittable.vtable = &sphere_vtable;
        sphere->origin = pos;
        sphere->radius = radius;
        sphere->surface_color = surface_color;

        return sphere;
}

void sphere_destroy(struct sphere *sphere)
{
        free(sphere);
}
