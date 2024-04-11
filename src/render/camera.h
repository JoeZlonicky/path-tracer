#pragma once

#include "../hittables/hittable.h"
#include "../math/ray.h"
#include "../math/vector_3.h"
#include "image.h"

class Camera {
public:
	[[nodiscard]] Image render(const Hittable& world);

	Point3 pos{0.0, 0.0, -1.0};
	Point3 look_at{0.0, 0.0, 0.0};
	Vector3 up{0.0, 1.0, 0.0};

	double aspect_ratio = 1.0;
	double vfov = 90.0;
	double defocus_angle = 0.0;
	double focus_distance = 10.0;

	int image_width = 100;
	int samples_per_pixel = 10;
	int max_bounces = 10;

	Color background_primary{1.0, 1.0, 1.0};
	Color background_secondary{0.5, 0.7, 1.0};

private:
	void init();

	[[nodiscard]] Ray calc_ray(int x, int y) const;
	[[nodiscard]] Color calc_ray_color(const Ray& r, int bounces_left, const Hittable& world);

	[[nodiscard]] Point3 pixel_random_sample() const;
	[[nodiscard]] Point3 defocus_disk_sample() const;
	[[nodiscard]] Color background_color(const Ray& r) const;

	int _image_height = 0;
	Point3 _pixel_upper_left;
	Vector3 _pixel_delta_u;
	Vector3 _pixel_delta_v;
	Vector3 _basis_u;
	Vector3 _basis_v;
	Vector3 _basis_w;
	Vector3 _defocus_disk_u;
	Vector3 _defocus_disk_v;
};