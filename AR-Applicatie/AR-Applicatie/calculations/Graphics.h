#pragma once

#include <vector>

#include "Math.h"

class Graphics {
private:
public:

	struct triangle {
		Math::vec3d p[3];
		Math::vec2d vt[3];
		Math::vec3d vn[3];
	};

	struct mesh {
		std::vector<triangle> tris;

	};


	static Math::vec3d triangle_getNormal(triangle tri);
};
