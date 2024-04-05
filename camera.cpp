#include "camera.h"

#include "utility.h"

void Camera::render(const Hittable& world, std::ostream& out)
{
	init();

	out << "P3\n" << image_width << ' ' << image_height << "\n255\n";

	for (int y = 0; y < image_height; ++y) {
		std::clog << "\rRows remaining: " << (image_height - y) << ' ' << std::flush;
		for (int x = 0; x < image_width; ++x) {
			Color pixel_color{ 0.0, 0.0, 0.0 };
			for (int sample = 0; sample < samples_per_pixel; ++sample) {
				auto r = get_ray(x, y);
				pixel_color += ray_color(r, max_bounces, world);
			}
			
			write_color_256(out, pixel_color, samples_per_pixel);
		}
	}
	std::clog << "\rDone.            \n";
}

void Camera::init()
{
	image_height = static_cast<int>(image_width / aspect_ratio);
	image_height = (image_height < 1) ? 1 : image_height;

	// Viewport
	double focal_length = (pos - look_at).magnitude();
	auto theta = degrees_to_radians(vfov);
	auto h = tan(theta / 2.0);
	auto viewport_height = 2.0 * h * focal_length;
	// Don't use aspect ratio here for width, as that is only the preferred ratio, need to use image width/height for actual ratio
	double viewport_width = viewport_height * (static_cast<double>(image_width) / image_height);

	basis_w = (pos - look_at).normalized();
	basis_u = up.cross(basis_w).normalized();
	basis_v = basis_w.cross(basis_u);

	auto viewport_u = viewport_width * basis_u;
	auto viewport_v = viewport_height * -basis_v;

	pixel_delta_u = viewport_u / image_width;
	pixel_delta_v = viewport_v / image_height;

	auto viewport_upper_left = pos - (focal_length * basis_w) - viewport_u / 2.0 - viewport_v / 2.0;
	pixel_upper_left = viewport_upper_left + (pixel_delta_u + pixel_delta_v) * 0.5;
}

Color Camera::ray_color(const Ray& r, int bounces_left, const Hittable& world) const
{
	if (bounces_left <= 0) return { 0.0, 0.0, 0.0 };

	HitRecord record;
	static const double min_travel = 0.0001;
	if (world.hit(r, Interval(min_travel, infinity), record)) {
		Ray scattered;
		Color attenuation;
		if (record.mat->scatter(r, record, attenuation, scattered)) {
			return attenuation * ray_color(scattered, bounces_left - 1, world);
		}
		return { 0, 0, 0 };
	}

	Vector3 unit_direction = r.getDirection().normalized();
	auto a = (unit_direction.y + 1.0) * 0.5;
	return Color(1.0, 1.0, 1.0) * (1.0 - a) + Color(0.5, 0.7, 1.0) * a;
}

Ray Camera::get_ray(int x, int y) const
{
	auto pixel_center = pixel_upper_left + (pixel_delta_u * x) + (pixel_delta_v * y);
	auto pixel_sample = pixel_center + pixel_random_sample();

	auto ray_origin = pos;
	auto ray_direction = pixel_sample - ray_origin;

	return { ray_origin, ray_direction };
}

Vector3 Camera::pixel_random_sample() const
{
	auto px = -0.5 + random_normalized();
	auto py = -0.5 + random_normalized();
	return (px * pixel_delta_u) + (px * pixel_delta_v);
}
