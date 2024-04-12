#include "material.h"

#include "../math/vector_3.h"

Color Material::emitted() const {
	return {0.0, 0.0, 0.0};
}
