#include "quad.h"

#include <cmath>
#include <memory>

#include "../math/aabb.h"
#include "../math/interval.h"
#include "../math/ray.h"
#include "../math/vector_3.h"
#include "hit_record.h"

Quad::Quad(const Point3& pos, const Vector3& u, const Vector3& v, std::shared_ptr<Material> material) : _pos(pos), _u(u), _v(v), _material(material) {
	auto n = _u.cross(_v);
	_normal = n.normalized();
	_d = _normal.dot(_pos);
	_w = n / n.dot(n);

	update_bounding_box();
}

AABB Quad::bounding_box() const {
	return _bbox;
}

bool Quad::hit(const Ray& r, const Interval& ray_t, HitRecord& record_out) const {
	auto normal_dot_direction = _normal.dot(r.getDirection());

	// Check for parallel to plane
	if(fabs(normal_dot_direction) < 1e-8) return false;

	auto t = (_d - _normal.dot(r.getOrigin())) / normal_dot_direction;
	if(!ray_t.contains(t)) return false;

	auto intersection = r.at(t);
	auto planar_hit = intersection - _pos;
	auto alpha = _w.dot(planar_hit.cross(_v));
	auto beta = _w.dot(_u.cross(planar_hit));

	if(!hit_local(alpha, beta, record_out)) return false;

	record_out.t = t;
	record_out.position = intersection;
	record_out.material = _material;
	record_out.set_face_normal(r, _normal);

	return true;
}

bool Quad::hit_local(double a, double b, HitRecord& record_out) const {
	return Interval::unit.contains(a) && Interval::unit.contains(b);
}

void Quad::update_bounding_box() {
	auto diagonal_a = AABB{_pos, _pos + _u + _v};
	auto diagonal_b = AABB{_pos + _u, _pos + _v};
	_bbox = {diagonal_a, diagonal_b};
	_bbox.pad_to_min_size(0.00001);
}
