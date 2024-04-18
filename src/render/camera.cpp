#include "camera.h"

#include <cmath>
#include <functional>
#include <iostream>
#include <memory>

#include "../hittables/hit_record.h"
#include "../hittables/hittable.h"
#include "../materials/material.h"
#include "../math/interval.h"
#include "../math/math_utility.h"
#include "../math/ray.h"
#include "../math/vector_3.h"
#include "grid_thread_pool.h"
#include "image.h"

Camera::Camera(std::shared_ptr<Hittable> scene) : _scene(scene), _thread_pool([&](int x, int y) {render_pixel(x, y); }) {
}

Camera::~Camera() {
}

void Camera::render(bool quick) {
	init();

	_samples_per_pixel = quick ? low_quality_render_samples : high_quality_render_samples;
	_render = std::make_shared<Image>(image_width, _image_height);
	for (int y = 0; y < _image_height; ++y) {
		for (int x = 0; x < image_width; ++x) {
			_thread_pool.queue_task({ x, y });
		}
	}
}

bool Camera::is_rendering() {
	return _thread_pool.get_n_remaining_tasks() > 0;
}

int Camera::get_n_pixel_renders_remaining() {
	return _thread_pool.get_n_remaining_tasks();
}

std::shared_ptr<Image> Camera::get_render() {
	return _render;
}

void Camera::init() {
	_image_height = static_cast<int>(image_width / aspect_ratio);
	_image_height = (_image_height < 1) ? 1 : _image_height;

	// Viewport
	auto theta = MathUtility::degrees_to_radians(vfov);
	auto h = tan(theta / 2.f);
	auto viewport_height = 2.f * h * focus_distance;
	// Don't use aspect ratio here for width, as that is only the preferred ratio, need to use image width/height for actual ratio
	auto viewport_width = viewport_height * (static_cast<float>(image_width) / _image_height);

	_basis_w = (pos - look_at).normalized();
	_basis_u = up.cross(_basis_w).normalized();
	_basis_v = _basis_w.cross(_basis_u);

	auto viewport_u = viewport_width * _basis_u;
	auto viewport_v = viewport_height * -_basis_v;

	_pixel_delta_u = viewport_u / static_cast<float>(image_width);
	_pixel_delta_v = viewport_v / static_cast<float>(_image_height);

	auto viewport_upper_left = pos - (focus_distance * _basis_w) - viewport_u / 2.f - viewport_v / 2.f;
	_pixel_upper_left = viewport_upper_left + (_pixel_delta_u + _pixel_delta_v) * 0.5f;

	auto defocus_radius = focus_distance * tan(MathUtility::degrees_to_radians(defocus_angle / 2.f));
	_defocus_disk_u = _basis_u * defocus_radius;
	_defocus_disk_v = _basis_v * defocus_radius;
}

void Camera::render_pixel(int x, int y) {
	Color pixel_color{ 0.f, 0.f, 0.f };
	for (int sample = 0; sample < _samples_per_pixel; ++sample) {
		auto r = calc_ray(x, y);
		pixel_color += calc_ray_color(r, max_bounces);
	}
	_render->set_pixel(x, y, pixel_color / static_cast<float>(_samples_per_pixel));
}

Ray Camera::calc_ray(int x, int y) const {
	auto pixel_center = _pixel_upper_left + (_pixel_delta_u * static_cast<float>(x)) + (_pixel_delta_v * static_cast<float>(y));
	auto pixel_sample = pixel_center + pixel_random_sample();

	auto ray_origin = (defocus_angle <= 0.f) ? pos : defocus_disk_sample();
	auto ray_direction = pixel_sample - ray_origin;

	return { ray_origin, ray_direction };
}

Color Camera::calc_ray_color(const Ray& r, int bounces_left) {
	if (bounces_left <= 0) return { 0.f, 0.f, 0.f };

	HitRecord record;
	constexpr auto min_travel = 0.001f;
	if (!_scene->hit(r, Interval(min_travel, MathUtility::infinity), record)) {
		return background_color(r);
	}

	Ray scattered;
	Color attenuation;
	Color emission_color = record.material->emitted();

	if (!record.material->scatter(r, record, attenuation, scattered)) {
		return emission_color;
	}

	Color scatter_color = attenuation * calc_ray_color(scattered, bounces_left - 1);
	return emission_color + scatter_color;
}

Point3 Camera::pixel_random_sample() const {
	auto px = -0.5f + MathUtility::random_normalized();
	auto py = -0.5f + MathUtility::random_normalized();
	return (px * _pixel_delta_u) + (px * _pixel_delta_v);
}

Point3 Camera::defocus_disk_sample() const {
	auto p = MathUtility::random_in_unit_disk();
	return pos + p.x * _defocus_disk_u + p.y * _defocus_disk_v;
}

Color Camera::background_color(const Ray& r) const {
	auto unit_direction = r.getDirection().normalized();
	auto a = (unit_direction.y + 1.f) * 0.5f;
	return (1.f - a) * background_primary + a * background_secondary;
}
