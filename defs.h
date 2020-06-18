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

struct hit_record {
	struct vec3 hit_point;
	struct vec3 normal;
	float t;
};

struct ray {
        struct vec3 origin;
        struct vec3 dir;
};

struct sphere {
        struct vec3 origin;
        float radius;
};

// vec.c
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
struct ray ray_to_pixel(const struct camera *cam, const struct image *img, int i, int j);
int ray_sphere_intersection(const struct ray *ray,
		            const struct sphere *sphere,
		            float t_0, float t_1,
		            struct hit_record *record);
struct vec3 ray_at(const struct ray *ray, float t);

// image.c
struct image *image_create(int width, int height);
void image_destroy(struct image *img);
int image_set_pixel(struct image *img, int x, int y, color c);
void image_write_ppm(char *filename, const struct image *img);

//camera.c
struct camera *camera_create(struct vec3 position, float focal_length);
void camera_destroy(struct camera *cam);
void camera_look_at(struct camera *cam, struct vec3 target);

#endif // DEFS_H
