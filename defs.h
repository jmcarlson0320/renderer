#ifndef DEFS_H
#define DEFS_H

#include <stdio.h>

#define ERROR -1
#define TRUE 1
#define FALSE 0

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

struct ray {
        struct vec3 origin;
        struct vec3 dir;
};

struct directional_light {
        struct vec3 dir;
        color intensity;
};

struct hit_record {
	struct vec3 hit_point;
	struct vec3 normal;
        color surface_color;
	float t;
        int front_face;
};

// this is an implementation of polymorphism
// see home/jmc/programming/vtable_in_c/my_attempt/ for another example
// hittable is the "base class", derived class structs need to have
// a hittable struct as their first member.
// when creating a derived class instance, load the vtable pointer with
// a pointer to the derived class's static vtable that has pointers to the
// derived class's implementation.
struct hittable {
        const struct vtable *vtable;
        int id;
        struct hittable *next;
};

struct vtable {
        int (*hit)(const struct hittable *hittable, const struct ray *ray, float t_0, float t_1, struct hit_record *record);
};

struct hittable_list {
        struct hittable *head;
        int num_elements;
};

struct sphere {
        struct hittable hittable;
        struct vec3 origin;
        float radius;
        color surface_color;
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
int in_range_inclusive(float x, float min, float max);
float rand_float();
float max(float a, float b);
void write_color(FILE *fs, color c);
struct ray ray_to_pixel(const struct camera *cam, const struct image *img, int i, int j);
struct vec3 ray_at(const struct ray *ray, float t);

// image.c
struct image *image_create(int width, int height);
void image_destroy(struct image *img);
int image_set_pixel(struct image *img, int x, int y, color c);
void image_write_ppm(char *filename, const struct image *img);

// camera.c
struct camera *camera_create(struct vec3 position, float focal_length);
void camera_destroy(struct camera *cam);
void camera_look_at(struct camera *cam, struct vec3 target);

// sphere.c
struct sphere *sphere_create(struct vec3 pos, float radius, color surface_color);
void sphere_destroy(struct sphere *sphere);

// hittable.c
int hittable_hit(const struct hittable *hittable, const struct ray *ray, float t_0, float t_1, struct hit_record *record);
struct hittable_list *hittable_list_create();
void hittable_list_destroy(struct hittable_list *list);
int hittable_list_add(struct hittable_list *list, struct hittable *element);
int hittable_list_remove(struct hittable_list *list, struct hittable *element);
int hittable_list_hit(const struct hittable_list *list, const struct ray *ray, float t_0, float t_1, struct hit_record *record);

#endif // DEFS_H
