#include <chrono>
#include <cstdint>
#include <memory>
#include <ratio>
#include <thread>

#include "editor/editor.h"
#include "editor/window.h"
#include "hittables/hittable_list.h"
#include "render/camera.h"
#include "scenes/scenes.h"

int main() {
	// Setup
	Window window{ "Raytracer", 1280, 720 };
	Editor editor{ window };
	auto scene = std::make_shared<HittableList>(Scenes::five_spheres());
	auto camera = std::make_shared<Camera>();
	camera->set_scene(scene);
	editor.set_camera(camera);

	// 60FPS limit
	auto frame_time = std::chrono::duration<int64_t, std::ratio<1, 60>>();
	auto next_frame = std::chrono::steady_clock::now() + frame_time;
	auto last_frame = next_frame - frame_time;

	// Editor loop
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