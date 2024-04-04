#include <iostream>
#include <fstream>
#include <math.h>

#include "color.h"
#include "ray.h"
#include "vector_3.h"


static double sphere_check(const Point3& center, double radius, const Ray& r) {
	auto oc = r.getOrigin() - center;
	auto dir = r.getDirection();
	auto r2 = radius * radius;

	// https://raytracing.github.io/books/RayTracingInOneWeekend.html#addingasphere/ray-sphereintersection
	auto a = dir.squared_magnitude();
	auto half_b = oc.dot(dir);
	auto c = oc.squared_magnitude() - r2;

	auto discriminant = half_b * half_b - a * c;
	if (discriminant < 0.0) {
		return -1.0;
	}
	else {
		return (-half_b - sqrt(discriminant)) / a;
	}
	
}

static Color ray_color(const Ray& r) {
	auto t = sphere_check(Point3{ 0.0, 0.0, -1.0 }, 0.5, r);
	if (t > 0.0) {
		auto normal = (r.at(t) - Vector3(0.0, 0.0, -1.0)).normalized();
		return 0.5 * Color(normal.x + 1.0, normal.y + 1.0, normal.z + 1.0);
	}

	Vector3 unit_direction = r.getDirection().normalized();
	auto a = (unit_direction.y + 1.0) * 0.5;
	return Color(1.0, 1.0, 1.0) * (1.0 - a) + Color(0.5, 0.7, 1.0) * a;
}

int main() {
	double aspect_ratio = 16.0 / 9.0;

	int image_width = 400;
	int image_height = static_cast<int>(image_width / aspect_ratio);
	image_height = (image_height < 1) ? 1 : image_height;

	double viewport_height = 2.0;  // Arbitrary
	// Don't use aspect ratio here for width, as that is only the preferred ratio, need to use image width/height for actual ratio
	double viewport_width = viewport_height * (static_cast<double>(image_width) / image_height);  

	double focal_length = 1.0;
	auto camera_center = Point3(0, 0, 0);

	auto viewport_u = Vector3(viewport_width, 0, 0);
	auto viewport_v = Vector3(0, -viewport_height, 0);

	auto pixel_delta_u = viewport_u / image_width;
	auto pixel_delta_v = viewport_v / image_height;

	auto viewport_upper_left = camera_center - Vector3(0, 0, focal_length) - viewport_u / 2.0 - viewport_v / 2.0;
	auto pixel_upper_left = viewport_upper_left + (pixel_delta_u + pixel_delta_v) * 0.5;

	std::ofstream output;
	output.open("output.ppm");

	output << "P3\n" << image_width << ' ' << image_height << "\n255\n";

	for (int y = 0; y < image_height; ++y) {
		std::clog << "\rRows remaining: " << (image_height - y) << ' ' << std::flush;
		for (int x = 0; x < image_width; ++x) {
			auto pixel_center = pixel_upper_left + (pixel_delta_u * x) + (pixel_delta_v * y);
			auto ray_direction = pixel_center - camera_center;
			Ray r {camera_center, ray_direction};

			Color pixel_color = ray_color(r);
			write_color_256(output, pixel_color);
		}
	}

	output.close();
	std::clog << "\rDone.            \n";
	return 0;
}