#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"

namespace createObjects2D 
{

	Mesh* CreateBar(const std::string& name, glm::vec3 leftBottomCorner, float health, glm::vec3 color, bool fill = false);
	Mesh* createQuad();
}
