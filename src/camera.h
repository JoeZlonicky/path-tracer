#pragma once

#include <vector>

#include "color.h"
#include "hittable.h"
#include "ray.h"


class Camera {
public:
	void render(const Hittable& world, std::vector<std::vector<Color>>& output);

	Point3 pos{ 0.0, 0.0, -1.0 };
	Point3 look_at{ 0.0, 0.0, 0.0 };
	Vector3 up{ 0.0, 1.0, 0.0 };

	double aspect_ratio = 1.0;
	double vfov = 90.0;
	double defocus_angle = 0.0;
	double focus_distance = 10.0;
	
	int image_width = 100;
	int samples_per_pixel = 10;
	int max_bounces = 10;

private:
	void init();
	Ray get_ray(int x, int y) const;
	Point3 pixel_random_sample() const;
	Point3 defocus_disk_sample() const;

	int image_height;
	Point3 pixel_upper_left;
	Vector3 pixel_delta_u;
	Vector3 pixel_delta_v;
	Vector3 basis_u, basis_v, basis_w;
	Vector3 defocus_disk_u;
	Vector3 defocus_disk_v;
};