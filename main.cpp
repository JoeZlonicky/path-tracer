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
	// Scene
	HittableList world;
	for (int x = -5; x < 5; ++x) {
		for (int y = -5; y < 5; ++y) {
			for (int z = -5; z < 5; ++z) {
				auto choose_mat = random_normalized();
				Point3 center{ double(x), double(y), double(z) };

				std::shared_ptr<Material> sphere_material;

				if (choose_mat < 0.8) {
					// Diffuse
					auto albedo = Color::random() * Color::random();
					sphere_material = std::make_shared<Lambertian>(albedo);
					
				}
				else if (choose_mat < 0.95) {
					// Metal
					auto albedo = Color::random(0.5, 1.0);
					auto fuzz = random_range(0.0, 0.5);
					sphere_material = std::make_shared<Metal>(albedo, fuzz);
				}
				else {
					// Glass
					sphere_material = std::make_shared<Dielectric>(1.5);
				}

				world.add(std::make_shared<Sphere>(center, 0.2, sphere_material));
			}
			
		}
	}

	// Camera
	Camera cam;
	cam.aspect_ratio = 16.0 / 9.0;
	cam.image_width = 600;
	cam.max_bounces = 10;
	cam.samples_per_pixel = 20;

	cam.vfov = 20.0;
	cam.pos = { 13.0, -5.0, 3.0 };
	cam.look_at = { 0.0, 0.0, 0.0 };
	cam.up = { 0.0, 1.0, 0.0 };

	cam.defocus_angle = 0.6;
	cam.focus_distance = 10.0;

	// Render
	std::ofstream output;
	output.open("output.ppm");
	cam.render(world, output);
	output.close();

	return 0;
}