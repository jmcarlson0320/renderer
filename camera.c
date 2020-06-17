#include "defs.h"

struct camera *camera_create(struct vec3 position, float focal_length)
{
	struct camera *cam = malloc(sizeof(struct camera));
	cam->pos = position;
	cam->focal_length = focal_length;
	cam->u = vec3(1.0f, 0.0f, 0.0f);
	cam->v = vec3(0.0f, 1.0f, 0.0f);
	cam->w = vec3(0.0f, 0.0f, 1.0f);
}

void camera_destroy(struct camera *cam)
{
	free(cam);
}

void camera_look_at(struct camera *cam, struct vec3 target)
{
	vec3_sub(&cam->w, &cam->pos, &target);
	vec3_normalize(&cam->w, &cam->w);

	struct vec3 ref = vec3(0.0f, 1.0f, 0.0f);
	vec3_cross(&cam->u, &ref, &cam->w);
	vec3_normalize(&cam->u, &cam->u);

	vec3_cross(&cam->v, &cam->w, &cam->u);
}
