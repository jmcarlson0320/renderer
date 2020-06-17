#ifndef DEFS_H
#define DEFS_H

#include <stdio.h>

#define ERROR -1

enum axis {
	X_COOR,
	Y_COOR,
	Z_COOR
};

enum color_comp {
	RED,
	GREEN,
	BLUE
};

struct vec2 {
	float e[2];
};

struct vec3 {
	float e[3];
};

// aliases
typedef struct vec3 color;

struct image {
	int width;
	int height;
	color *pixels;
};

struct camera {
	struct vec3 u;
	struct vec3 v;
	struct vec3 w;
	struct vec3 pos;
	float focal_length;
};

struct ray {
	struct vec3 origin;
	struct vec3 dir;
};

// vec.c
struct vec2 vec2(float x, float y);
void vec2_add(struct vec2 *dest, const struct vec2 *u, const struct vec2 *v);
void vec2_sub(struct vec2 *dest, const struct vec2 *u, const struct vec3 *v);
void vec2_mult(struct vec2 *dest, const struct vec2 *v, float s);
void vec2_div(struct vec2 *dest, const struct vec2 *v, float s);
float vec2_dot(const struct vec2 *u, const struct vec2 *v);
float vec2_len_sqr(const struct vec2 *v);
float vec2_len(const struct vec2 *v);
void vec2_normalize(struct vec2 *dest, const struct vec2 *v);

struct vec3 vec3(float x, float y, float z);
void vec3_add(struct vec3 *dest, const struct vec3 *u, const struct vec3 *v);
void vec3_sub(struct vec3 *dest, const struct vec3 *u, const struct vec3 *v);
void vec3_mult(struct vec3 *dest, const struct vec3 *v, float s);
void vec3_div(struct vec3 *dest, const struct vec3 *v, float s);
float vec3_dot(const struct vec3 *u, const struct vec3 *v);
void vec3_cross(struct vec3 *dest, const struct vec3 *u, const struct vec3 *v);
float vec3_len_sqr(const struct vec3 *v);
float vec3_len(const struct vec3 *v);
void vec3_normalize(struct vec3 *dest, const struct vec3 *v);

// utils.c
float lerp(float norm, float min, float max);
float norm(float value, float min, float max);
float map(float value, float srcMin, float srcMax, float destMin, float destMax);
void write_color(FILE *fs, color c);
struct ray ray_to_pixel(struct camera *cam, struct image *img, int i, int j);

// image.c
struct image *image_create(int width, int height);
void image_destroy(struct image *img);
int image_set_pixel(struct image *img, int x, int y, color c);
void image_write_ppm(char *filename, const struct image *img);

//camera.c
struct camera *camera_create(struct vec3 position, struct vec3 dir, float focal_length);
void camera_destroy(struct camera *cam);
void camera_look_at(struct camera *cam, struct vec3 target);

#endif // DEFS_H
