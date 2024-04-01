#include <iostream>
#include <fstream>

int main() {
	int image_width = 256;
	int image_height = 256;

	std::ofstream output;
	output.open("output.ppm");

	output << "P3\n" << image_width << ' ' << image_height << "\n255\n";

	for (int y = 0; y < image_height; ++y) {
		for (int x = 0; x < image_width; ++x) {
			double r = double(x) / (image_width - 1);
			double g = double(y) / (image_height - 1);
			double b = 0;

			int ir = static_cast<int>(255.999 * r);
			int ig = static_cast<int>(255.999 * g);
			int ib = static_cast<int>(255.999 * b);
			
			output << ir << ' ' << ig << ' ' << ib << '\n';
		}
	}

	output.close();
	return 0;
}