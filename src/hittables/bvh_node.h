#pragma once

#include <memory>
#include <vector>

#include "../math/aabb.h"
#include "../math/interval.h"
#include "../math/ray.h"
#include "hit_record.h"
#include "hittable.h"
#include "hittable_list.h"

class BVHNode : public Hittable {
public:
	BVHNode(HittableList list);
	BVHNode(std::vector<std::shared_ptr<Hittable>>& objects, size_t start, size_t end);

	bool hit(const Ray& r, const Interval& ray_t, HitRecord& record_out) const override;
	[[nodiscard]] AABB bounding_box() const override;

private:
	std::shared_ptr<Hittable> _left;
	std::shared_ptr<Hittable> _right;
	AABB _bbox;
};