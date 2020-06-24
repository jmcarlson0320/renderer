#include "defs.h"
#include <stdio.h>
#include <stdlib.h>
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

float rand_float()
{
        return (float) rand() / (float) (RAND_MAX);
}

float max(float a, float b)
{
        return a > b ? a : b;
}

void write_color(FILE *fs, color c)
{
        int ir = (int) map(c.e[RED], 0.0f, 1.0f, 0.0f, 255.0f);
        int ig = (int) map(c.e[GREEN], 0.0f, 1.0f, 0.0f, 255.0f);
        int ib = (int) map(c.e[BLUE], 0.0f, 1.0f, 0.0f, 255.0f);

        fprintf(fs, "%d\t%d\t%d\n", ir, ig, ib);
}

struct vec3 ray_at(const struct ray *ray, float t)
{
        // P(t) = A + bt
        struct vec3 point;
        vec3_mult(&point, &ray->dir, t);
        vec3_add(&point, &point, &ray->origin);

        return point;
}

struct ray ray_to_pixel(const struct camera *cam, const struct image *img, int i, int j)
{
        float aspect_ratio = (float) img->width / (float) img->height;
        float l = -1.0 * aspect_ratio;
        float r = aspect_ratio;
        float t = 1.0f;
        float b = -1.0f;

        float u = l + (r - l) * ((float) i + rand_float()) / img->width;
        float v = b + (t - b) * ((float) j + rand_float()) / img->height;

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
