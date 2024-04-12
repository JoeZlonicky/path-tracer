#include <cmath>
#include <fstream>
#include <iostream>
#include <memory>

#include "hittables/bvh_node.h"
#include "hittables/hittable_list.h"
#include "hittables/quad.h"
#include "hittables/sphere.h"
#include "materials/dielectric.h"
#include "materials/diffuse_light.h"
#include "materials/lambertian.h"
#include "materials/material.h"
#include "materials/metal.h"
#include "math/math_utility.h"
#include "math/vector_3.h"
#include "render/camera.h"
#include "render/window.h"

namespace {
	void create_sphere(HittableList& scene, const Point3& position, double r, std::shared_ptr<Material> material) {
		scene.add(std::make_shared<Sphere>(position, r, material));
	}
}

int main() {
	HittableList scene;

	// Ground
	auto ground_material = std::make_shared<Lambertian>(Color{0.13, 0.69, 0.3});
	create_sphere(scene, {0.0, -1002.0, 0.0}, 1000.0, ground_material);

	// 5 Spheres placed in a trapezoid shape
	auto distance = 1.0;
	auto radius = 0.8;
	auto y_offset = -0.25;

	auto tri_x = cos(MathUtility::pi / 6.0) * distance;
	auto tri_y = sin(MathUtility::pi / 6.0) * distance;

	//auto gold_metal_material = std::make_shared<Metal>(Color{1.0, 0.78, 0.08}, 0.05);
	//create_sphere(scene, {-tri_x * 2.0, distance + y_offset, 0.0}, radius, gold_metal_material);

	auto red_diffuse_material = std::make_shared<Lambertian>(Color{0.9, 0.1, 0.1});
	create_sphere(scene, {-tri_x, -tri_y + y_offset, 0.0}, radius, red_diffuse_material);

	auto green_diffuse_material = std::make_shared<Lambertian>(Color{0.71, 0.9, 0.11});
	create_sphere(scene, {0.0, distance + y_offset, 0.0}, radius, green_diffuse_material);

	auto blue_diffuse_material = std::make_shared<Lambertian>(Color{0.0, 0.6, 0.9});
	create_sphere(scene, {tri_x, -tri_y + y_offset, 0.0}, radius, blue_diffuse_material);

	//auto silver_metal_material = std::make_shared<Metal>(Color{0.9, 0.9, 0.9}, 0.3);
	//create_sphere(scene, {tri_x * 2.0, distance + y_offset, 0.0}, radius, silver_metal_material);

	auto white_light_material = std::make_shared<DiffuseLight>(Color{4.0, 4.0, 4.0});
	scene.add(std::make_shared<Quad>(Point3{-2.0, -2.0, -2.0}, Vector3{4.0, 0.0, 0.0}, Vector3{0.0, 4.0, 0.0}, white_light_material));

	// Put everything into a BVH tree
	scene = {std::make_shared<BVHNode>(scene)};

	// Camera
	Camera cam;
	cam.aspect_ratio = 16.0 / 9.0;
	cam.image_width = 400;
	cam.max_bounces = 50;
	cam.samples_per_pixel = 50;

	cam.vfov = 25.0;
	cam.pos = {0, 0.0, 10.0};
	cam.look_at = {0.0, 0.0, 0.0};
	cam.up = {0.0, 1.0, 0.0};

	cam.defocus_angle = 0.6;
	cam.focus_distance = 10.0;
	cam.background_primary = {0.0, 0.0, 0.0};
	cam.background_secondary = {0.0, 0.0, 0.0};

	// Render
	auto image = cam.render(scene);

	// Save
	std::ofstream output;
	constexpr auto output_filepath = "output.ppm";
	output.open(output_filepath);

	std::clog << "Saving output..." << std::endl;
	output << "P3\n" << image.get_width() << ' ' << image.get_height() << "\n255\n";
	for(auto& pixel : image.get_pixels()) {
		MathUtility::write_color_256(output, pixel);
	}

	output.close();
	std::clog << "Saved to " << output_filepath << std::endl;

	// Display window
	std::clog << "Displaying output window..." << std::endl;
	Window window{};
	bool success = window.init("Output");
	if(!success) return 1;

	success = window.load_image(output_filepath);
	if(!success) return 1;

	window.resize_window_to_image_size();

	while(window.should_keep_open()) {
		window.draw();
	}

	return 0;
}