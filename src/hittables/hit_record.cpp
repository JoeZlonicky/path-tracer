#include "hit_record.h"

#include "../math/ray.h"
#include "../math/vector_3.h"

void HitRecord::set_face_normal(const Ray& r, const Vector3& outward_normal) {
	is_front_face = r.getDirection().dot(outward_normal) < 0.f;
	normal = is_front_face ? outward_normal : -outward_normal;
}
