#include "hittable_list.h"

#include <memory>

#include "../math/aabb.h"
#include "../math/interval.h"
#include "../math/ray.h"
#include "hit_record.h"
#include "hittable.h"

HittableList::HittableList(std::shared_ptr<Hittable> object) {
	add(object);
}

void HittableList::clear() {
	objects.clear();
}

void HittableList::add(std::shared_ptr<Hittable> object) {
	objects.push_back(object);
	_bbox = {_bbox, object->bounding_box()};
}

bool HittableList::hit(const Ray& r, const Interval& ray_t, HitRecord& record_out) const {
	HitRecord temp_record;
	auto hit_anything = false;
	auto closest_so_far = ray_t.max;

	for(const auto& object : objects) {
		if(object->hit(r, Interval(ray_t.min, closest_so_far), temp_record)) {
			hit_anything = true;
			closest_so_far = temp_record.t;
			record_out = temp_record;
		}
	}

	return hit_anything;
}

AABB HittableList::bounding_box() const {
	return _bbox;
}
