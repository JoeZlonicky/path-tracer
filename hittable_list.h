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

	bool hit(const Ray& r, double ray_t_min, double ray_t_max, HitRecord& record) const override;
};