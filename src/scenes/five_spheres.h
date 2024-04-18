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
	static void create_sphere(HittableList& scene, const Point3& position, float r, std::shared_ptr<Material> material) {
		scene.add(std::make_shared<Sphere>(position, r, material));
	}
}

static HittableList five_spheres() {
	HittableList scene;

	// Ground
	auto ground_material = std::make_shared<Lambertian>(Color{0.13f, 0.69f, 0.3f});
	create_sphere(scene, {0.f, -1002.f, 0.f}, 1000.f, ground_material);

	// 5 Spheres placed in a trapezoid shape
	auto distance = 1.f;
	auto radius = 0.8f;
	auto y_offset = -0.25f;

	auto tri_x = cos(MathUtility::pi / 6.f) * distance;
	auto tri_y = sin(MathUtility::pi / 6.f) * distance;

	auto gold_metal_material = std::make_shared<Metal>(Color{1.f, 0.78f, 0.08f}, 0.05f);
	create_sphere(scene, {-tri_x * 2.f, distance + y_offset, 0.f}, radius, gold_metal_material);

	auto red_diffuse_material = std::make_shared<Lambertian>(Color{0.9f, 0.1f, 0.1f});
	create_sphere(scene, {-tri_x, -tri_y + y_offset, 0.f}, radius, red_diffuse_material);

	auto green_diffuse_material = std::make_shared<Lambertian>(Color{0.71f, 0.9f, 0.11f});
	create_sphere(scene, {0.f, distance + y_offset, 0.f}, radius, green_diffuse_material);

	auto blue_diffuse_material = std::make_shared<Lambertian>(Color{0.f, 0.6f, 0.9f});
	create_sphere(scene, {tri_x, -tri_y + y_offset, 0.f}, radius, blue_diffuse_material);

	auto silver_metal_material = std::make_shared<Metal>(Color{0.9f, 0.9f, 0.9f}, 0.3f);
	create_sphere(scene, {tri_x * 2.f, distance + y_offset, 0.f}, radius, silver_metal_material);

	return scene;
}