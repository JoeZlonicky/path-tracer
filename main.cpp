#include <iostream>
#include <fstream>

#include "color.h"
#include "vector_3.h"

int main() {
	int image_width = 256;
	int image_height = 256;

	std::ofstream output;
	output.open("output.ppm");

	output << "P3\n" << image_width << ' ' << image_height << "\n255\n";

	for (int y = 0; y < image_height; ++y) {
		std::clog << "\rRows remaining: " << (image_height - y) << ' ' << std::flush;
		for (int x = 0; x < image_width; ++x) {
			auto pixel_color = Color(double(x) / (image_width - 1), double(y) / (image_height - 1), 0);
			write_color_256(output, pixel_color);
		}
	}

	output.close();
	std::clog << "\rDone.            \n";
	return 0;
}