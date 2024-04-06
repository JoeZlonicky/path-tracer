#include <iostream>
#include <fstream>
#include <math.h>
#include <memory>

#include "camera.h"
#include "color.h"
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

	// Ground
	auto ground_material = std::make_shared<Lambertian>(Color{ 0.13, 0.69, 0.3 });
	create_sphere(world, 0.0, -1002.0, 0.0, 1000.0, ground_material);

	// 5 Spheres placed in a trapezoid shape
	auto distance = 1.0;
	auto radius = 0.8;
	auto y_offset = -0.25;

	auto tri_x = cos(pi / 6.0) * distance;
	auto tri_y = sin(pi / 6.0) * distance;
	
	auto red_diffuse_material = std::make_shared<Lambertian>(Color{ 0.9, 0.1, 0.1 });
	create_sphere(world, -tri_x, -tri_y + y_offset, 0.0, radius, red_diffuse_material);

	auto gold_metal_material = std::make_shared<Metal>(Color{ 1.0, 0.78, 0.08 }, 0.05);
	create_sphere(world, tri_x, -tri_y + y_offset, 0.0, radius, gold_metal_material);

	auto glass_material = std::make_shared<Dielectric>(1.5);
	create_sphere(world, 0.0, distance + y_offset, 0.0, radius, glass_material);

	auto blue_diffuse_material = std::make_shared<Lambertian>(Color{ 0.0, 0.6, 0.9 });
	create_sphere(world, -tri_x * 2.0, distance + y_offset, 0.0, radius, blue_diffuse_material);

	auto silver_metal_material = std::make_shared<Metal>(Color{ 0.9, 0.9, 0.9 }, 0.3);
	create_sphere(world, tri_x * 2.0, distance + y_offset, 0.0, radius, silver_metal_material);
	
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
	std::vector<std::vector<Color>> image;
	cam.render(world, image);

	std::ofstream output;
	output.open("output.ppm");
	output << "P3\n" << image[0].size() << ' ' << image.size() << "\n255\n";
	for (auto& row : image) {
		for (auto& value : row) {
			write_color_256(output, value);
		}
	}
	
	output.close();

	return 0;
}