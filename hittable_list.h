#pragma once

#include "hittable.h"

#include <memory>
#include <vector>

class HittableList : public Hittable {
public:
	std::vector<std::shared_ptr<Hittable>> objects;

	HittableList() {}
	HittableList(std::shared_ptr<Hittable> object);

	void clear();
	void add(std::shared_ptr<Hittable> object);

	bool hit(const Ray& r, Interval ray_t, HitRecord& record) const override;
};