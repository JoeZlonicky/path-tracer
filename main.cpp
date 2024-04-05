#include <iostream>
#include <fstream>
#include <math.h>
#include <memory>

#include "camera.h"
#include "hittable_list.h"
#include "sphere.h"
#include "lambertian.h"
#include "metal.h"
#include "dielectric.h"
#include "utility.h"

static void create_sphere(HittableList& world, double x, double y, double z, double r, std::shared_ptr<Material> mat) {
	world.add(std::make_shared<Sphere>(Point3(x, y, z), r, mat));
}

int main() {
	// World
	HittableList world;
	auto ground_mat = std::make_shared<Lambertian>(Color{ 0.5, 0.5, 0.5 });
	create_sphere(world, 0.0, -1000.0, -0.0, 1000.0, ground_mat);

	for (int a = -11; a < 11; ++a) {
		for (int b = -11; b < 11; ++b) {
			auto choose_mat = random_normalized();
			Point3 center{ a + 0.9 * random_normalized(), 0.2, b + 0.9 * random_normalized() };

			if ((center - Vector3{ 4.0, 0.2, 0.0 }).magnitude() > 0.9) {
				std::shared_ptr<Material> sphere_material;

				if (choose_mat < 0.8) {
					// Diffuse
					auto albedo = Color::random() * Color::random();
					sphere_material = std::make_shared<Lambertian>(albedo);
					world.add(std::make_shared<Sphere>(center, 0.2, sphere_material));
				}
				else if (choose_mat < 0.95) {
					// Metal
					auto albedo = Color::random(0.5, 1.0);
					auto fuzz = random_range(0.0, 0.5);
					sphere_material = std::make_shared<Metal>(albedo, fuzz);
					world.add(std::make_shared<Sphere>(center, 0.2, sphere_material));
				}
				else {
					// Glass
					sphere_material = std::make_shared<Dielectric>(1.5);
					world.add(std::make_shared<Sphere>(center, 0.2, sphere_material));
				}
			}
		}
	}

	auto mat_1 = std::make_shared<Dielectric>(1.5);
	create_sphere(world, 0.0, 1.0, 0.0, 1.0, mat_1);

	auto mat_2 = std::make_shared<Lambertian>(Color{ 0.4, 0.2, 0.1 });
	create_sphere(world, -4.0, 1.0, 0.0, 1.0, mat_2);

	auto mat_3 = std::make_shared<Metal>(Color{ 0.7, 0.6, 0.5 }, 0.0);
	create_sphere(world, 4.0, 1.0, -1.0, 1.0, mat_3);

	// Camera
	Camera cam;
	cam.aspect_ratio = 16.0 / 9.0;
	cam.image_width = 300;
	cam.max_bounces = 20;
	cam.samples_per_pixel = 10;

	cam.vfov = 20.0;
	cam.pos = { 13.0, 2.0, 3.0 };
	cam.look_at = { 0.0, 0.0, 0.0 };
	cam.up = { 0.0, 1.0, 0.0 };

	cam.defocus_angle = 0.6;
	cam.focus_distance = 10.0;

	std::ofstream output;
	output.open("output.ppm");
	cam.render(world, output);
	output.close();

	return 0;
}