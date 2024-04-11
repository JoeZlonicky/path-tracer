#pragma once

#include <memory>
#include <vector>

#include "../math/aabb.h"
#include "../math/interval.h"
#include "../math/ray.h"
#include "hit_record.h"
#include "hittable.h"

class HittableList : public Hittable {
public:
	std::vector<std::shared_ptr<Hittable>> objects;

	HittableList() {}
	HittableList(std::shared_ptr<Hittable> object);

	void clear();
	void add(std::shared_ptr<Hittable> object);

	bool hit(const Ray& r, const Interval& ray_t, HitRecord& record_out) const override;

	[[nodiscard]] AABB bounding_box() const override;

private:
	AABB _bbox;
};