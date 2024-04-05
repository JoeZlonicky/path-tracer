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
	// Materials
	auto ground_mat = std::make_shared<Lambertian>(Color{ 0.8, 0.8, 0.0 });
	auto center_mat = std::make_shared<Lambertian>(Color{ 0.0, 0.8, 0.8 });
	auto left_mat = std::make_shared<Dielectric>(1.5);
	auto right_mat = std::make_shared<Metal>(Color{ 0.8, 0.6, 0.2 }, 0.2);


	// World
	HittableList world;
	create_sphere(world, 0.0, -100.5, -1, 100.0, ground_mat);
	create_sphere(world, -1.0, 0.0, -1.0, 0.5, center_mat);
	create_sphere(world, 0.0, 0.0, -1.0, 0.5, left_mat);
	create_sphere(world, 1.0, 0.0, -1.0, 0.5, right_mat);

	// Camera
	Camera cam;
	cam.aspect_ratio = 16.0 / 9.0;
	cam.image_width = 400;
	cam.max_bounces = 50;
	cam.samples_per_pixel = 10;
	cam.vfov = 20.0;
	cam.pos = { -2, 2, 1 };
	cam.look_at = { 0.0, 0.0, -1.0 };
	cam.up = { 0.0, 1.0, 0.0 };

	std::ofstream output;
	output.open("output.ppm");
	cam.render(world, output);
	output.close();

	return 0;
}