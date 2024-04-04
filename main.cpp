#include <iostream>
#include <fstream>
#include <math.h>
#include <memory>

#include "camera.h"
#include "hittable_list.h"
#include "sphere.h"

static void create_sphere(HittableList& world, double x, double y, double z, double r) {
	world.add(std::make_shared<Sphere>(Point3(x, y, z), r));
}

int main() {
	// World
	HittableList world;
	create_sphere(world, 0.0, 0.0, -1.0, 0.5);
	create_sphere(world, 0.0, -100.5, -1, 100.0);

	// Camera
	Camera cam;
	cam.aspect_ratio = 16.0 / 9.0;
	cam.image_width = 400;
	cam.samples_per_pixel = 10;

	std::ofstream output;
	output.open("output.ppm");
	cam.render(world, output);
	output.close();

	return 0;
}