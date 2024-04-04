#include "hittable_list.h"

HittableList::HittableList(std::shared_ptr<Hittable> object)
{
    add(object);
}

void HittableList::clear()
{
    objects.clear();
}

void HittableList::add(std::shared_ptr<Hittable> object)
{
    objects.push_back(object);
}

bool HittableList::hit(const Ray& r, Interval ray_t, HitRecord& record) const
{
    HitRecord temp_record;
    bool hit_anything = false;
    auto closest_so_far = ray_t.max;

    for (const auto& object : objects) {
        if (object->hit(r, Interval(ray_t.min, closest_so_far), temp_record)) {
            hit_anything = true;
            closest_so_far = temp_record.t;
            record = temp_record;
        }
    }

    return hit_anything;
}
