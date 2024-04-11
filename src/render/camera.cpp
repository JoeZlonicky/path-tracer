#include "camera.h"

#include <cmath>
#include <iostream>

#include "../hittables/hit_record.h"
#include "../hittables/hittable.h"
#include "../materials/material.h"
#include "../math/interval.h"
#include "../math/math_utility.h"
#include "../math/ray.h"
#include "../math/vector_3.h"
#include "grid_thread_pool.h"
#include "image.h"

Image Camera::render(const Hittable& world) {
	init();

	Image output{image_width, _image_height};

	auto render_pixel = [&](int x, int y) {
		Color pixel_color{0.0, 0.0, 0.0};
		for(int sample = 0; sample < samples_per_pixel; ++sample) {
			auto r = calc_ray(x, y);
			pixel_color += calc_ray_color(r, max_bounces, world);
		}
		output.set_pixel(x, y, pixel_color / samples_per_pixel);
		};

	std::clog << "Starting render..." << std::endl;
	{
		GridThreadPool pool{render_pixel};
		for(int y = 0; y < _image_height; ++y) {
			for(int x = 0; x < image_width; ++x) {
				pool.queue_task({x, y});
			}
		}

		const auto total_n_tasks = image_width * _image_height;
		int current_percent = 0;
		// Note that when there are 0 remaining tasks, the threads are still wrapping up their work
		// The work will be guarenteed to be finished when the thread pool leaves scope, as all threads are joined
		while(auto n = pool.get_n_remaining_tasks()) {
			auto percent = static_cast<int>((1.0 - double(n) / total_n_tasks) * 100.0);
			if(percent > current_percent) {
				current_percent = percent;
				std::clog << current_percent << '%' << std::endl;
			}
		}
	}

	std::clog << "Render completed!" << std::endl;
	return output;
}

void Camera::init() {
	_image_height = static_cast<int>(image_width / aspect_ratio);
	_image_height = (_image_height < 1) ? 1 : _image_height;

	// Viewport
	auto theta = MathUtility::degrees_to_radians(vfov);
	auto h = tan(theta / 2.0);
	auto viewport_height = 2.0 * h * focus_distance;
	// Don't use aspect ratio here for width, as that is only the preferred ratio, need to use image width/height for actual ratio
	auto viewport_width = viewport_height * (static_cast<double>(image_width) / _image_height);

	_basis_w = (pos - look_at).normalized();
	_basis_u = up.cross(_basis_w).normalized();
	_basis_v = _basis_w.cross(_basis_u);

	auto viewport_u = viewport_width * _basis_u;
	auto viewport_v = viewport_height * -_basis_v;

	_pixel_delta_u = viewport_u / image_width;
	_pixel_delta_v = viewport_v / _image_height;

	auto viewport_upper_left = pos - (focus_distance * _basis_w) - viewport_u / 2.0 - viewport_v / 2.0;
	_pixel_upper_left = viewport_upper_left + (_pixel_delta_u + _pixel_delta_v) * 0.5;

	auto defocus_radius = focus_distance * tan(MathUtility::degrees_to_radians(defocus_angle / 2.0));
	_defocus_disk_u = _basis_u * defocus_radius;
	_defocus_disk_v = _basis_v * defocus_radius;
}

Ray Camera::calc_ray(int x, int y) const {
	auto pixel_center = _pixel_upper_left + (_pixel_delta_u * x) + (_pixel_delta_v * y);
	auto pixel_sample = pixel_center + pixel_random_sample();

	auto ray_origin = (defocus_angle <= 0) ? pos : defocus_disk_sample();
	auto ray_direction = pixel_sample - ray_origin;

	return {ray_origin, ray_direction};
}

Color Camera::calc_ray_color(const Ray& r, int bounces_left, const Hittable& world) {
	if(bounces_left <= 0) return {0.0, 0.0, 0.0};

	HitRecord record;
	constexpr auto min_travel = 0.0001;
	if(world.hit(r, Interval(min_travel, MathUtility::infinity), record)) {
		Ray scattered;
		Color attenuation;
		if(record.material->scatter(r, record, attenuation, scattered)) {
			return attenuation * calc_ray_color(scattered, bounces_left - 1, world);
		}
		return {0, 0, 0};
	}

	return background_color(r);
}

Point3 Camera::pixel_random_sample() const {
	auto px = -0.5 + MathUtility::random_normalized();
	auto py = -0.5 + MathUtility::random_normalized();
	return (px * _pixel_delta_u) + (px * _pixel_delta_v);
}

Point3 Camera::defocus_disk_sample() const {
	auto p = MathUtility::random_in_unit_disk();
	return pos + p.x * _defocus_disk_u + p.y * _defocus_disk_v;
}

Color Camera::background_color(const Ray& r) const {
	auto unit_direction = r.getDirection().normalized();
	auto a = (unit_direction.y + 1.0) * 0.5;
	return (1.0 - a) * background_primary + a * background_secondary;
}
