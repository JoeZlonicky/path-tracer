#pragma once

#include <cmath>
#include <memory>

#include "../hittables/hittable_list.h"
#include "../hittables/quad.h"
#include "../hittables/sphere.h"
#include "../materials/dielectric.h"
#include "../materials/diffuse_light.h"
#include "../materials/lambertian.h"
#include "../materials/material.h"
#include "../materials/metal.h"
#include "../math/math_utility.h"
#include "../math/vector_3.h"

namespace {
	static void instance_sphere(HittableList& scene, const Point3& position, float r, std::shared_ptr<Material> material) {
		scene.add(std::make_shared<Sphere>(position, r, material));
	}

	static void instance_quad(HittableList& scene, const Point3& bottom_left, const Vector3& u, const Vector3& v, std::shared_ptr<Material> material) {
		scene.add(std::make_shared<Quad>(bottom_left, u, v, material));
	}
}

namespace Scenes {

	static const char* scene_names[] = {
		"5 spheres",
		"Pyramid",
		"Room"
	};

	static const char* material_names[] = {
		"Default"
	};

	static std::shared_ptr<HittableList> instance_scene(int i);

	static HittableList five_spheres() {
		HittableList scene;

		// Ground
		auto ground_material = std::make_shared<Lambertian>(Color{ 0.13f, 0.69f, 0.3f });
		instance_sphere(scene, { 0.f, -1002.f, 0.f }, 1000.f, ground_material);

		// 5 Spheres placed in a trapezoid shape
		auto distance = 1.f;
		auto radius = 0.8f;
		auto y_offset = -0.25f;

		auto tri_x = cos(MathUtility::pi / 6.f) * distance;
		auto tri_y = sin(MathUtility::pi / 6.f) * distance;

		auto gold_metal_material = std::make_shared<Metal>(Color{ 1.f, 0.78f, 0.08f }, 0.05f);
		instance_sphere(scene, { -tri_x * 2.f, distance + y_offset, 0.f }, radius, gold_metal_material);

		auto red_diffuse_material = std::make_shared<Lambertian>(Color{ 0.9f, 0.1f, 0.1f });
		instance_sphere(scene, { -tri_x, -tri_y + y_offset, 0.f }, radius, red_diffuse_material);

		auto green_diffuse_material = std::make_shared<Lambertian>(Color{ 0.71f, 0.9f, 0.11f });
		instance_sphere(scene, { 0.f, distance + y_offset, 0.f }, radius, green_diffuse_material);

		auto blue_diffuse_material = std::make_shared<Lambertian>(Color{ 0.f, 0.6f, 0.9f });
		instance_sphere(scene, { tri_x, -tri_y + y_offset, 0.f }, radius, blue_diffuse_material);

		auto silver_metal_material = std::make_shared<Metal>(Color{ 0.9f, 0.9f, 0.9f }, 0.3f);
		instance_sphere(scene, { tri_x * 2.f, distance + y_offset, 0.f }, radius, silver_metal_material);

		return scene;
	}

	static HittableList pyramid() {
		HittableList scene;

		// Ground
		auto ground_material = std::make_shared<Lambertian>(Color{ 0.13f, 0.69f, 0.3f });
		instance_sphere(scene, { 0.f, -1004.f, 0.f }, 1000.f, ground_material);

		// Pyramid of spheres
		auto radius = 1.f;
		auto diameter = radius * 2.f;
		auto height = 5;

		auto sphere_material = std::make_shared<Metal>(Color{ 1.f, 0.8f, 0.05f }, 0.f);
		auto pyramid_bottom_center = Point3{ 0.f, -4.f, -15.f };
		auto y_offset = radius;

		for (auto level = 0; level < height; ++level) {
			auto n = height - level;
			auto y = static_cast<float>(level) * sqrtf(2.f) * radius;
			auto x_offset = -(n - 1) * radius;
			auto z_offset = x_offset;

			for (auto x = 0; x < n; ++x) {
				for (auto z = 0; z < n; ++z) {
					Point3 pos{ x * diameter + x_offset, y + y_offset, z * diameter + z_offset };
					pos += pyramid_bottom_center;
					instance_sphere(scene, pos, radius, sphere_material);
				}
			}
		}

		return scene;
	}

	static HittableList room() {
		HittableList scene;

		// Room
		auto start_z = -10.f;
		auto end_z = -35.f;
		auto center_z = (start_z + end_z) / 2.f;
		auto room_depth = start_z - end_z;

		auto width = 10.f;
		auto half_width = width / 2.f;
		auto height = 10.f;
		auto half_height = height / 2.f;
		auto floor_y = -half_height;

		auto white_material = std::make_shared<Lambertian>(Color{ 1.f, 1.f, 1.f });

		auto left_wall_material = std::make_shared<Lambertian>(Color{ 0.8f, 0.13f, 0.3f });
		instance_quad(scene, { -half_width, -half_height, start_z }, { 0.f, height, 0.f }, { 0.f, 0.f, -room_depth }, left_wall_material);

		auto right_wall_material = std::make_shared<Lambertian>(Color{ 0.13f, 0.69f, 0.3f });
		instance_quad(scene, { half_width, -half_height, start_z }, { 0.f, height, 0.f }, { 0.f, 0.f, -room_depth }, right_wall_material);

		auto& back_wall_material = white_material;
		instance_quad(scene, { -half_width, -half_height, end_z }, { 0.f, height, 0.f }, { width, 0.f, 0.f }, back_wall_material);

		auto& floor_material = white_material;
		instance_quad(scene, { -half_width, -half_height, start_z }, { width, 0.f, 0.f }, { 0.f, 0.f, -room_depth }, floor_material);

		auto& ceiling_material = white_material;
		instance_quad(scene, { -half_width, half_height, start_z }, { width, 0.f, 0.f }, { 0.f, 0.f, -room_depth }, ceiling_material);

		// Light
		auto light_material = std::make_shared<DiffuseLight>(Color{ 5.f, 5.f, 5.f });
		instance_sphere(scene, { 0.f, half_height, -22.5f }, 2.f, light_material);

		// Spheres
		auto big_sphere_radius = 2.2f;
		auto small_sphere_radius = 1.2f;

		auto& sphere_material = white_material;
		instance_sphere(scene, { -2.5f, floor_y + big_sphere_radius, center_z - 2.5f }, big_sphere_radius, sphere_material);
		instance_sphere(scene, { 2.5f, floor_y + small_sphere_radius, center_z + 2.5f }, small_sphere_radius, sphere_material);

		return scene;
	}

	static std::shared_ptr<HittableList> instance_scene(int i) {
		HittableList scene;
		if (i == 0) scene = five_spheres();
		else if (i == 1) scene = pyramid();
		else if (i == 2) scene = room();
		else assert(false);

		return std::make_shared<HittableList>(scene);
	}
}