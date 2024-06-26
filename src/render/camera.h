#pragma once

#include <memory>

#include "../hittables/bvh_node.h"
#include "../hittables/hittable.h"
#include "../hittables/hittable_list.h"
#include "../math/ray.h"
#include "../math/vector_3.h"
#include "image.h"
#include "multithreaded_task.h"

class Camera {
public:
	Camera();
	~Camera() = default;

	void set_scene(std::shared_ptr<HittableList> scene);
	void render(bool quick = false);
	void stop_render();
	[[nodiscard]] bool is_rendering();
	[[nodiscard]] int get_n_pixel_renders_remaining();  // Returns -1 if render ended prematurely (e.g. stopped)
	[[nodiscard]] std::shared_ptr<Image> get_render();

	Point3 pos{ 0.f, 1.f, 10.f };
	Point3 look_at{ 0.f, 0.f, 0.f };
	Vector3 up{ 0.f, 1.f, 0.f };

	float aspect_ratio = 16.f / 9.f;
	float vfov = 25.f;
	float defocus_angle = 0.6f;
	float focus_distance = 10.f;

	int low_quality_render_samples = 1;
	int high_quality_render_samples = 100;
	int image_width = 400;
	int max_bounces = 50;

	Color background_primary{ 1.f, 1.f, 1.f };
	Color background_secondary{ 0.5f, 0.7f, 1.f };

private:
	void init();
	void render_pixel(int i);

	[[nodiscard]] Ray calc_ray(int x, int y) const;
	[[nodiscard]] Color calc_ray_color(const Ray& r, int bounces_left);

	[[nodiscard]] Point3 pixel_random_sample() const;
	[[nodiscard]] Point3 defocus_disk_sample() const;
	[[nodiscard]] Color background_color(const Ray& r) const;

	std::unique_ptr<BVHNode> _scene_tree = nullptr;
	std::shared_ptr<Image> _render = nullptr;
	MultithreadedTask _render_task;

	int _samples_per_pixel = 10;
	bool _is_rendering = false;
	int _image_height = 0;
	Point3 _pixel_upper_left;
	Vector3 _pixel_delta_u;
	Vector3 _pixel_delta_v;
	Vector3 _basis_u;
	Vector3 _basis_v;
	Vector3 _basis_w;
	Vector3 _defocus_disk_u;
	Vector3 _defocus_disk_v;
};