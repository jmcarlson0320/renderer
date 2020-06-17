#include "defs.h"
#include <stdio.h>

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

void write_color(FILE *fs, color c)
{
	int ir = (int) map(c.e[RED], 0.0f, 1.0f, 0.0f, 255.0f);
	int ig = (int) map(c.e[GREEN], 0.0f, 1.0f, 0.0f, 255.0f);
	int ib = (int) map(c.e[BLUE], 0.0f, 1.0f, 0.0f, 255.0f);

	fprintf(fs, "%d\t%d\t%d\n", ir, ig, ib);
}

struct ray ray_to_pixel(struct camera *cam, struct image *img, int i, int j)
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
