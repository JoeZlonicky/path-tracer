#pragma once

#include <memory>

#include "../math/aabb.h"
#include "../math/interval.h"
#include "../math/ray.h"
#include "../math/vector_3.h"
#include "hit_record.h"
#include "hittable.h"

class Quad : public Hittable {
public:
	Quad(const Point3& pos, const Vector3& u, const Vector3& v, std::shared_ptr<Material> material);

	AABB bounding_box() const override;

	bool hit(const Ray& r, const Interval& ray_t, HitRecord& record_out) const override;
	virtual bool hit_local(float a, float b, HitRecord& record_out) const;

private:
	virtual void update_bounding_box();

	Point3 _pos;
	Vector3 _u, _v;
	AABB _bbox;
	Vector3 _normal;
	std::shared_ptr<Material> _material;

	float _d;  // plane constant
	Vector3 _w;  // quad constant for intersection
};

