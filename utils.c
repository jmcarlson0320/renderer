#include "defs.h"
#include <stdio.h>
#include <math.h>

float lerp(float norm, float min, float max)
{
        return (max - min) * norm + min;
}

float norm(float value, float min, float max)
{
        return (value - min) / (max - min);
}

float map(float value, float srcMin, float srcMax, float destMin, float destMax)
{
        float n = norm(value, srcMin, srcMax);
        return lerp(n, destMin, destMax);
}

int in_range_inclusive(float x, float min, float max)
{
        return min <= x && x <= max;
}

void write_color(FILE *fs, color c)
{
        int ir = (int) map(c.e[RED], 0.0f, 1.0f, 0.0f, 255.0f);
        int ig = (int) map(c.e[GREEN], 0.0f, 1.0f, 0.0f, 255.0f);
        int ib = (int) map(c.e[BLUE], 0.0f, 1.0f, 0.0f, 255.0f);

        fprintf(fs, "%d\t%d\t%d\n", ir, ig, ib);
}

struct ray ray_to_pixel(const struct camera *cam, const struct image *img, int i, int j)
{
        float aspect_ratio = (float) img->width / (float) img->height;
        float l = -1.0 * aspect_ratio;
        float r = aspect_ratio;
        float t = 1.0f;
        float b = -1.0f;

        float u = l + (r - l) * ((float) i + 0.5f) / img->width;
        float v = b + (t - b) * ((float) j + 0.5f) / img->height;

        //ray direction: -dW + uU + vV
        struct vec3 dir;
        vec3_mult(&dir, &cam->w, (-1.0f) * cam->focal_length);

        struct vec3 tmp;
        vec3_mult(&tmp, &cam->u, u);
        vec3_add(&dir, &dir, &tmp);

        vec3_mult(&tmp, &cam->v, v);
        vec3_add(&dir, &dir, &tmp);

        struct ray result;
        result.origin = cam->pos;
        result.dir = dir;

        return result;
}

int ray_sphere_intersection(const struct ray *ray,
                const struct sphere *sphere,
                float t_0, float t_1,
                struct hit_record *record)
{
        float a;
        float b;
        float c;
        float discriminant;

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

        if (vec3_dot(&record->normal, &ray->dir) > 0) {
                vec3_mult(&record->normal, &record->normal, -1.0f);
                record->front_face = FALSE;
        } else {
                record->front_face = TRUE;
        }

        return TRUE;
}

struct vec3 ray_at(const struct ray *ray, float t)
{
        // P(t) = A + bt
        struct vec3 point;
        vec3_mult(&point, &ray->dir, t);
        vec3_add(&point, &point, &ray->origin);

        return point;
}
