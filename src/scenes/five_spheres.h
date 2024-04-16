#pragma once

#include <cmath>
#include <memory>

#include "../hittables/hittable_list.h"
#include "../hittables/sphere.h"
#include "../materials/lambertian.h"
#include "../materials/material.h"
#include "../materials/metal.h"
#include "../math/math_utility.h"
#include "../math/vector_3.h"

namespace {
	static void create_sphere(HittableList& scene, const Point3& position, double r, std::shared_ptr<Material> material) {
		scene.add(std::make_shared<Sphere>(position, r, material));
	}
}

static HittableList five_spheres() {
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

	return scene;
}