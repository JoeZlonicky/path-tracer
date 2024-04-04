#pragma once

#include <iostream>
#include "vector_3.h"

using Color = Vector3;

void write_color_256(std::ostream& out, Color pixel_color, int samples_per_pixel);