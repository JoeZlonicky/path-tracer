#include "bvh_node.h"

#include <algorithm>
#include <memory>
#include <vector>

#include "../math/aabb.h"
#include "../math/interval.h"
#include "../math/ray.h"
#include "hit_record.h"
#include "hittable.h"
#include "hittable_list.h"

namespace {
	bool box_compare(const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b, int axis_index) {
		auto a_axis_interval = a->bounding_box().intervals[axis_index];
		auto b_axis_interval = b->bounding_box().intervals[axis_index];
		return a_axis_interval.min < b_axis_interval.min;
	}

	bool box_x_compare(const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b) {
		return box_compare(a, b, 0);
	}

	bool box_y_compare(const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b) {
		return box_compare(a, b, 1);
	}

	bool box_z_compare(const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b) {
		return box_compare(a, b, 2);
	}
}

BVHNode::BVHNode(HittableList list) : BVHNode(list.objects, 0, list.objects.size()) {
}

BVHNode::BVHNode(std::vector<std::shared_ptr<Hittable>>& objects, size_t start, size_t end) {
	_bbox = AABB::empty;
	for (auto i = start; i < end; ++i) {
		_bbox = { _bbox, objects[i]->bounding_box() };
	}

	auto span = end - start;
	if (span == 1) {
		_left = _right = objects[start];
	}
	else if (span == 2) {
		_left = objects[start];
		_right = objects[start + 1];
	}
	else {
		auto axis = _bbox.longest_axis();
		auto comperator = (axis == 0) ? box_x_compare
			: (axis == 1) ? box_y_compare
			: box_z_compare;
		std::sort(objects.begin() + start, objects.begin() + end, comperator);

		auto mid = start + span / 2;
		_left = std::make_shared<BVHNode>(objects, start, mid);
		_right = std::make_shared<BVHNode>(objects, mid, end);
	}
}

bool BVHNode::hit(const Ray& r, const Interval& ray_t, HitRecord& record_out) const {
	if (!_bbox.intersect(r, ray_t)) return false;

	auto hit_left = _left->hit(r, ray_t, record_out);
	auto hit_right = _right->hit(r, { ray_t.min, hit_left ? record_out.t : ray_t.max }, record_out);

	return hit_left || hit_right;
}

AABB BVHNode::bounding_box() const {
	return _bbox;
}
