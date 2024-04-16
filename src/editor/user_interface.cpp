#include "user_interface.h"

#include "../math/math_utility.h"
#include "../render/camera.h"
#include <fstream>
#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_sdlrenderer2.h>
#include <iostream>
#include <memory>

namespace {
	void save_output(std::shared_ptr<Image> image) {
		std::ofstream output;
		constexpr auto output_filepath = "output.ppm";
		output.open(output_filepath);

		std::clog << "Saving output..." << std::endl;
		output << "P3\n" << image->get_width() << ' ' << image->get_height() << "\n255\n";
		for(auto& pixel : image->get_pixels()) {
			MathUtility::write_color_256(output, pixel);
		}

		output.close();
		std::clog << "Saved to " << output_filepath << std::endl;
	}
}

UserInterface::UserInterface(SDL_Window* window, SDL_Renderer* renderer) : _window(window), _renderer(renderer) {
	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	_io = &ImGui::GetIO();
	_io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

	ImGui_ImplSDL2_InitForSDLRenderer(_window, _renderer);
	ImGui_ImplSDLRenderer2_Init(_renderer);
}

UserInterface::~UserInterface() {
	ImGui_ImplSDLRenderer2_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
}

bool UserInterface::init_was_successful() {
	return false;
}

void UserInterface::process_event(const SDL_Event* e) {
	ImGui_ImplSDL2_ProcessEvent(e);
}

void UserInterface::update() {
	ImGui_ImplSDLRenderer2_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	{
		ImGui::SetNextWindowPos(ImVec2{0.0, 0.0});
		ImGui::SetNextWindowSize(ImVec2{200.0, 720});
		ImGui::Begin("Hello, world!", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
		if(ImGui::Button("Render") && _camera != nullptr && !_camera->is_rendering()) {
			std::cout << "Render" << std::endl;
			_camera->render();
		}
		if(_camera != nullptr) {
			if(_camera->is_rendering()) {
				ImGui::SameLine();
				auto text = "Rendering..." + std::to_string(_camera->get_n_pixel_renders_remaining());
				ImGui::Text(text.c_str());
			} else if(_camera->get_render()) {
				if(ImGui::Button("Save")) {
					save_output(_camera->get_render());
				}
			}
		}
		ImGui::End();
	}

	ImGui::Render();
	ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
}

void UserInterface::set_camera(std::shared_ptr<Camera> camera) {
	_camera = camera;
}
