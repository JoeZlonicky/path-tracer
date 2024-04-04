#include "camera.h"

#include "utility.h"

void Camera::render(const Hittable& world, std::ostream& out)
{
	init();

	out << "P3\n" << image_width << ' ' << image_height << "\n255\n";

	for (int y = 0; y < image_height; ++y) {
		std::clog << "\rRows remaining: " << (image_height - y) << ' ' << std::flush;
		for (int x = 0; x < image_width; ++x) {
			auto pixel_center = pixel_upper_left + (pixel_delta_u * x) + (pixel_delta_v * y);
			auto ray_direction = pixel_center - center;
			Ray r{ center, ray_direction };

			Color pixel_color = ray_color(r, world);
			write_color_256(out, pixel_color);
		}
	}
	std::clog << "\rDone.            \n";
}

void Camera::init()
{
	image_height = static_cast<int>(image_width / aspect_ratio);
	image_height = (image_height < 1) ? 1 : image_height;

	double viewport_height = 2.0;  // Arbitrary
	// Don't use aspect ratio here for width, as that is only the preferred ratio, need to use image width/height for actual ratio
	double viewport_width = viewport_height * (static_cast<double>(image_width) / image_height);

	// Camera
	double focal_length = 1.0;

	// Viewport
	auto viewport_u = Vector3(viewport_width, 0, 0);
	auto viewport_v = Vector3(0, -viewport_height, 0);

	pixel_delta_u = viewport_u / image_width;
	pixel_delta_v = viewport_v / image_height;

	auto viewport_upper_left = center - Vector3(0, 0, focal_length) - viewport_u / 2.0 - viewport_v / 2.0;
	pixel_upper_left = viewport_upper_left + (pixel_delta_u + pixel_delta_v) * 0.5;
}

Color Camera::ray_color(const Ray& r, const Hittable& world) const
{
	HitRecord record;
	if (world.hit(r, Interval(0.0, infinity), record)) {
		return 0.5 * (record.normal + Color(1.0, 1.0, 1.0));
	}

	Vector3 unit_direction = r.getDirection().normalized();
	auto a = (unit_direction.y + 1.0) * 0.5;
	return Color(1.0, 1.0, 1.0) * (1.0 - a) + Color(0.5, 0.7, 1.0) * a;
}
