#pragma once

#include "color.h"
#include "hittable.h"
#include "ray.h"


class Camera {
public:
	void render(const Hittable& world, std::ostream& out);

	Point3 center{ 0.0, 0.0, 0.0 };
	double aspect_ratio = 1.0;
	int image_width = 100;

private:
	void init();
	Color ray_color(const Ray& r, const Hittable& world) const;

	int image_height;
	Point3 pixel_upper_left;
	Vector3 pixel_delta_u;
	Vector3 pixel_delta_v;
};