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

bool HittableList::hit(const Ray& r, double ray_t_min, double ray_t_max, HitRecord& record) const
{
    HitRecord temp_record;
    bool hit_anything = false;
    auto closest_so_far = ray_t_max;

    for (const auto& object : objects) {
        if (object->hit(r, ray_t_min, closest_so_far, temp_record)) {
            hit_anything = true;
            closest_so_far = temp_record.t;
            record = temp_record;
        }
    }

    return hit_anything;
}
