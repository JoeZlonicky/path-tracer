#include <memory>

#include "editor/editor.h"
#include "editor/window.h"
#include "hittables/bvh_node.h"
#include "render/camera.h"
#include "scenes/five_spheres.h"
#include <chrono>

static void configure_camera(Camera& camera) {
	camera.aspect_ratio = 16.0 / 9.0;
	camera.image_width = 400;
	camera.max_bounces = 50;
	camera.samples_per_pixel = 100;

	camera.vfov = 25.0;
	camera.pos = { 0, 0.0, 10.0 };
	camera.look_at = { 0.0, 0.0, 0.0 };

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
	Window window{ "Raytracer", 1280, 720 };
	Editor editor{ window };
	editor.set_camera(camera);

	auto frame_time = std::chrono::duration<int64_t, std::ratio<1, 60>>();
	auto next_frame = std::chrono::steady_clock::now() + frame_time;
	auto last_frame = next_frame - frame_time;

	while (true) {
		editor.update();
		if (!editor.should_keep_open()) break;

		window.clear();
		editor.draw();
		window.refresh();

		std::this_thread::sleep_until(next_frame);
		last_frame = next_frame;
		next_frame += frame_time;
	}

	return 0;
}