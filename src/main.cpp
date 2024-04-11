#include <fstream>
#include <iostream>
#include <math.h>
#include <memory>

#include "bvh_node.h"
#include "camera.h"
#include "dielectric.h"
#include "hittable_list.h"
#include "lambertian.h"
#include "metal.h"
#include "sphere.h"
#include "utility.h"
#include "window.h"

namespace {
	void create_sphere(HittableList& scene, const Point3& position, double r, std::shared_ptr<Material> material) {
		scene.add(std::make_shared<Sphere>(position, r, material));
	}
}

int main() {
	HittableList scene;

	// Ground
	auto ground_material = std::make_shared<Lambertian>(Color{ 0.13, 0.69, 0.3 });
	create_sphere(scene, { 0.0, -1002.0, 0.0 }, 1000.0, ground_material);

	// 5 Spheres placed in a trapezoid shape
	auto distance = 1.0;
	auto radius = 0.8;
	auto y_offset = -0.25;

	auto tri_x = cos(Utility::pi / 6.0) * distance;
	auto tri_y = sin(Utility::pi / 6.0) * distance;

	auto red_diffuse_material = std::make_shared<Lambertian>(Color{ 0.9, 0.1, 0.1 });
	create_sphere(scene, { -tri_x, -tri_y + y_offset, 0.0 }, radius, red_diffuse_material);

	auto gold_metal_material = std::make_shared<Metal>(Color{ 1.0, 0.78, 0.08 }, 0.05);
	create_sphere(scene, { tri_x, -tri_y + y_offset, 0.0 }, radius, gold_metal_material);

	auto glass_material = std::make_shared<Dielectric>(1.5);
	create_sphere(scene, { 0.0, distance + y_offset, 0.0 }, radius, glass_material);

	auto blue_diffuse_material = std::make_shared<Lambertian>(Color{ 0.0, 0.6, 0.9 });
	create_sphere(scene, { -tri_x * 2.0, distance + y_offset, 0.0 }, radius, blue_diffuse_material);

	auto silver_metal_material = std::make_shared<Metal>(Color{ 0.9, 0.9, 0.9 }, 0.3);
	create_sphere(scene, { tri_x * 2.0, distance + y_offset, 0.0 }, radius, silver_metal_material);

	// Put everything into a BVH tree
	scene = { std::make_shared<BVHNode>(scene) };

	// Camera
	Camera cam;
	cam.aspect_ratio = 16.0 / 9.0;
	cam.image_width = 400;
	cam.max_bounces = 50;
	cam.samples_per_pixel = 50;

	cam.vfov = 25.0;
	cam.pos = { 0, 0.0, 10.0 };
	cam.look_at = { 0.0, 0.0, 0.0 };
	cam.up = { 0.0, 1.0, 0.0 };

	cam.defocus_angle = 0.6;
	cam.focus_distance = 10.0;

	// Render
	auto image = cam.render(scene);

	// Save
	std::ofstream output;
	constexpr auto output_filepath = "output.ppm";
	output.open(output_filepath);

	std::clog << "Saving output..." << std::endl;
	output << "P3\n" << image.get_width() << ' ' << image.get_height() << "\n255\n";
	for (auto& pixel : image.get_pixels()) {
		Utility::write_color_256(output, pixel);
	}

	output.close();
	std::clog << "Saved to " << output_filepath << std::endl;

	// Display window
	std::clog << "Displaying output window..." << std::endl;
	Window window{};
	bool success = window.init("Output");
	if (!success) return 1;

	success = window.load_image(output_filepath);
	if (!success) return 1;

	window.resize_window_to_image_size();

	while (window.should_keep_open()) {
		window.draw();
	}

	return 0;
}