#include "defs.h"
#include <stdlib.h>

int hittable_hit(const struct hittable *hittable, const struct ray *ray,
                 float t_0, float t_1,
                 struct hit_record *record)
{
        return (hittable->vtable->hit)(hittable, ray, t_0, t_1, record);
}

struct hittable_list *hittable_list_create()
{
        struct hittable_list *list = malloc(sizeof(struct hittable_list));

        list->head = NULL;
        list->num_elements = 0;

        return list;
}

void hittable_list_destroy(struct hittable_list *list)
{
        free(list);
}

int hittable_list_add(struct hittable_list *list, struct hittable *element)
{
        element->next = list->head;
        list->head = element;
        list->num_elements++;

        return 0;
}

int hittable_list_remove(struct hittable_list *list, struct hittable *element)
{
        struct hittable *cur = list->head;
        struct hittable *prev = NULL;
        int found = FALSE;

        while (cur != NULL && !found) {
                if (cur->id == element->id && cur == list->head) {
                        found = TRUE;
                        list->head = list->head->next;
                        list->num_elements--;
                } else if (cur->id == element->id && cur != list->head) {
                        found = TRUE;
                        prev->next = cur->next;
                        list->num_elements--;
                }
                prev = cur;
                cur = cur->next;
        }

        return found;
}

int hittable_list_hit(const struct hittable_list *list, const struct ray *ray,
                      float t_0, float t_1,
                      struct hit_record *record)
{
        int hit = FALSE;
        struct hittable *cur = list->head;
        float t_max = t_1;

        while (cur != NULL) {
                if (hittable_hit(cur, ray, t_0, t_max, record) &&
                                in_range_inclusive(record->t, t_0, t_max)) {
                        hit = TRUE;
                        t_max = record->t;
                }

                cur = cur->next;
        }

        return hit;
}
