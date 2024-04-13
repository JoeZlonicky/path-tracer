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
#include "ui/window.h"

namespace {
	void create_sphere(HittableList& scene, const Point3& position, double r, std::shared_ptr<Material> material) {
		scene.add(std::make_shared<Sphere>(position, r, material));
	}

	std::shared_ptr<Hittable> create_scene() {
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

		auto gold_metal_material = std::make_shared<Metal>(Color{1.0, 0.78, 0.08}, 0.05);
		create_sphere(scene, {-tri_x * 2.0, distance + y_offset, 0.0}, radius, gold_metal_material);

		auto red_diffuse_material = std::make_shared<Lambertian>(Color{0.9, 0.1, 0.1});
		create_sphere(scene, {-tri_x, -tri_y + y_offset, 0.0}, radius, red_diffuse_material);

		auto green_diffuse_material = std::make_shared<Lambertian>(Color{0.71, 0.9, 0.11});
		create_sphere(scene, {0.0, distance + y_offset, 0.0}, radius, green_diffuse_material);

		auto blue_diffuse_material = std::make_shared<Lambertian>(Color{0.0, 0.6, 0.9});
		create_sphere(scene, {tri_x, -tri_y + y_offset, 0.0}, radius, blue_diffuse_material);

		auto silver_metal_material = std::make_shared<Metal>(Color{0.9, 0.9, 0.9}, 0.3);
		create_sphere(scene, {tri_x * 2.0, distance + y_offset, 0.0}, radius, silver_metal_material);

		// Put everything into a BVH tree
		auto scene_tree = std::make_shared<BVHNode>(scene);
		return scene_tree;
	}
}

int main() {
	// Setup
	auto scene_tree = create_scene();
	std::shared_ptr<Camera> camera = std::make_shared<Camera>(scene_tree);
	camera->aspect_ratio = 16.0 / 9.0;
	camera->image_width = 400;
	camera->max_bounces = 50;
	camera->samples_per_pixel = 10;

	camera->vfov = 25.0;
	camera->pos = {0, 0.0, 10.0};
	camera->look_at = {0.0, 0.0, 0.0};
	camera->up = {0.0, 1.0, 0.0};

	camera->defocus_angle = 0.6;
	camera->focus_distance = 10.0;

	// Display window
	Window window{"Raytracer", 1280, 720};
	if(!window.init_was_successful()) return 1;

	window.set_camera(camera);

	while(window.update()) {}

	return 0;
}