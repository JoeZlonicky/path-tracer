#include "material.h"

#include "../math/vector_3.h"

Color Material::emitted() const {
	return {0.f, 0.f, 0.f};
}
