#include "camera.h"

#include "thread_pool.h"
#include "utility.h"

Image Camera::render(const Hittable& world)
{
	init();

	Image output{ image_width, image_height };

	auto render_pixel = [&](int x, int y) {
		Color pixel_color{ 0.0, 0.0, 0.0 };
		for (int sample = 0; sample < samples_per_pixel; ++sample) {
			auto r = calc_ray(x, y);
			pixel_color += calc_ray_color(r, max_bounces, world);
		}
		output.set_pixel(x, y, pixel_color / samples_per_pixel);
		};

	ThreadPool pool{};

	std::clog << "Starting render..." << std::endl;
	for (int y = 0; y < image_height; ++y) {
		for (int x = 0; x < image_width; ++x) {
			pool.queue_task(std::bind(render_pixel, x, y));
		}
	}

	const auto total_n_tasks = image_width * image_height;
	int current_percent = 0;
	while (auto n = pool.get_n_remaining_tasks()) {
		auto percent = static_cast<int>((1.0 - double(n) / total_n_tasks) * 100.0);
		if (percent > current_percent) {
			current_percent = percent;
			std::clog << current_percent << '%' << std::endl;
		}
	}
	std::clog << "Render completed!" << std::endl;

	return output;
}

void Camera::init()
{
	image_height = static_cast<int>(image_width / aspect_ratio);
	image_height = (image_height < 1) ? 1 : image_height;

	// Viewport
	auto theta = Utility::degrees_to_radians(vfov);
	auto h = tan(theta / 2.0);
	auto viewport_height = 2.0 * h * focus_distance;
	// Don't use aspect ratio here for width, as that is only the preferred ratio, need to use image width/height for actual ratio
	auto viewport_width = viewport_height * (static_cast<double>(image_width) / image_height);

	basis_w = (pos - look_at).normalized();
	basis_u = up.cross(basis_w).normalized();
	basis_v = basis_w.cross(basis_u);

	auto viewport_u = viewport_width * basis_u;
	auto viewport_v = viewport_height * -basis_v;

	pixel_delta_u = viewport_u / image_width;
	pixel_delta_v = viewport_v / image_height;

	auto viewport_upper_left = pos - (focus_distance * basis_w) - viewport_u / 2.0 - viewport_v / 2.0;
	pixel_upper_left = viewport_upper_left + (pixel_delta_u + pixel_delta_v) * 0.5;

	auto defocus_radius = focus_distance * tan(Utility::degrees_to_radians(defocus_angle / 2.0));
	defocus_disk_u = basis_u * defocus_radius;
	defocus_disk_v = basis_v * defocus_radius;
}

Ray Camera::calc_ray(int x, int y) const
{
	auto pixel_center = pixel_upper_left + (pixel_delta_u * x) + (pixel_delta_v * y);
	auto pixel_sample = pixel_center + pixel_random_sample();

	auto ray_origin = (defocus_angle <= 0) ? pos : defocus_disk_sample();
	auto ray_direction = pixel_sample - ray_origin;

	return { ray_origin, ray_direction };
}

Color Camera::calc_ray_color(const Ray& r, int bounces_left, const Hittable& world)
{
	if (bounces_left <= 0) return { 0.0, 0.0, 0.0 };

	HitRecord record;
	constexpr auto min_travel = 0.0001;
	if (world.hit(r, Interval(min_travel, Utility::infinity), record)) {
		Ray scattered;
		Color attenuation;
		if (record.mat->scatter(r, record, attenuation, scattered)) {
			return attenuation * calc_ray_color(scattered, bounces_left - 1, world);
		}
		return { 0, 0, 0 };
	}

	return calc_background_color(r);
}

Point3 Camera::pixel_random_sample() const
{
	auto px = -0.5 + Utility::random_normalized();
	auto py = -0.5 + Utility::random_normalized();
	return (px * pixel_delta_u) + (px * pixel_delta_v);
}

Point3 Camera::defocus_disk_sample() const
{
	auto p = Utility::random_in_unit_disk();
	return pos + p.x * defocus_disk_u + p.y * defocus_disk_v;
}

Color Camera::calc_background_color(Ray r) const
{
	auto unit_direction = r.getDirection().normalized();
	auto a = (unit_direction.y + 1.0) * 0.5;
	return (1.0 - a) * background_primary + a * background_secondary;
}
