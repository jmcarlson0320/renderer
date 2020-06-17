#include "defs.h"
#include <math.h>

struct vec3 vec3(float x, float y, float z)
{
        struct vec3 v = {{x, y, z}};
        return v;
}

void vec3_add(struct vec3 *dest, const struct vec3 *u, const struct vec3 *v)
{
        dest->e[0] = u->e[0] + v->e[0];
        dest->e[1] = u->e[1] + v->e[1];
        dest->e[2] = u->e[2] + v->e[2];
}

void vec3_sub(struct vec3 *dest, const struct vec3 *u, const struct vec3 *v)
{
        dest->e[0] = u->e[0] - v->e[0];
        dest->e[1] = u->e[1] - v->e[1];
        dest->e[2] = u->e[2] - v->e[2];
}

void vec3_mult(struct vec3 *dest, const struct vec3 *v, float s)
{
        dest->e[0] = v->e[0] * s;
        dest->e[1] = v->e[1] * s;
        dest->e[2] = v->e[2] * s;
}

void vec3_div(struct vec3 *dest, const struct vec3 *v, float s)
{
        dest->e[0] = v->e[0] / s;
        dest->e[1] = v->e[1] / s;
        dest->e[2] = v->e[2] / s;
}

float vec3_dot(const struct vec3 *u, const struct vec3 *v)
{
        return u->e[0] * v->e[0] +
                u->e[1] * v->e[1] +
                u->e[2] * v->e[2];
}

void vec3_cross(struct vec3 *dest, const struct vec3 *u, const struct vec3 *v)
{
        dest->e[0] = u->e[1] * v->e[2] - u->e[2] * v->e[1];
        dest->e[1] = u->e[2] * v->e[0] - u->e[0] * v->e[2];
        dest->e[2] = u->e[0] * v->e[1] - u->e[1] * v->e[0];
}

float vec3_len_sqr(const struct vec3 *v)
{
        return v->e[0] * v->e[0] +
                v->e[1] * v->e[1] +
                v->e[2] * v->e[2];
}

float vec3_len(const struct vec3 *v)
{
        return sqrt(vec3_len_sqr(v));
}

void vec3_normalize(struct vec3 *dest, const struct vec3 *v)
{
        vec3_div(dest, v, vec3_len(v));
}
