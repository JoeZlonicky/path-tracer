#include <iostream>
#include <fstream>
#include <math.h>
#include <memory>

#include "camera.h"
#include "hittable_list.h"
#include "sphere.h"
#include "lambertian.h"
#include "metal.h"

static void create_sphere(HittableList& world, double x, double y, double z, double r, std::shared_ptr<Material> mat) {
	world.add(std::make_shared<Sphere>(Point3(x, y, z), r, mat));
}

int main() {
	// Materials
	auto ground_mat = std::make_shared<Lambertian>(Color{ 0.8, 0.8, 0.0 });
	auto center_mat = std::make_shared<Lambertian>(Color{ 0.7, 0.3, 0.3 });
	auto left_metal_mat = std::make_shared<Metal>(Color{ 0.8, 0.8, 0.8 });
	auto right_metal_mat = std::make_shared<Metal>(Color{ 0.8, 0.6, 0.2 });

	// World
	HittableList world;
	create_sphere(world, 0.0, -100.5, -1, 100.0, ground_mat);
	create_sphere(world, 0.0, 0.0, -1.0, 0.5, center_mat);
	create_sphere(world, -1.0, 0.0, -1.0, 0.5, left_metal_mat);
	create_sphere(world, 1.0, 0.0, -1.0, 0.5, right_metal_mat);

	// Camera
	Camera cam;
	cam.aspect_ratio = 16.0 / 9.0;
	cam.image_width = 400;
	cam.max_bounces = 20;
	cam.samples_per_pixel = 10;

	std::ofstream output;
	output.open("output.ppm");
	cam.render(world, output);
	output.close();

	return 0;
}