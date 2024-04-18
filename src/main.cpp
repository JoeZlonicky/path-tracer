#include <memory>

#include "editor/window.h"
#include "hittables/bvh_node.h"
#include "render/camera.h"
#include "scenes/five_spheres.h"

static void configure_camera(Camera& camera) {
	camera.aspect_ratio = 16.0 / 9.0;
	camera.image_width = 400;
	camera.max_bounces = 50;
	camera.samples_per_pixel = 100;

	camera.vfov = 25.0;
	camera.pos = {0, 0.0, 10.0};
	camera.look_at = {0.0, 0.0, 0.0};

	camera.defocus_angle = 0.6;
	camera.focus_distance = 10.0;
}

int main() {
	// Setup
	auto scene = five_spheres();
	auto scene_tree = std::make_shared<BVHNode>(scene);

	auto camera = std::make_shared<Camera>(scene_tree);
	configure_camera(*camera);

	// Display window
	Window window{"Raytracer", 1280, 720};
	window.set_camera(camera);

	while(window.update()) {}

	return 0;
}