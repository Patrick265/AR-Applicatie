#pragma once

#include <vector>

#include "Math.h"

class Graphics {
private:
public:

	struct triangle {
		//vertices
		Math::vec3d p[3];
		//texture coordinates
		Math::vec2d vt[3];
		//vertex normals
		Math::vec3d vn[3];

		//Face normal
		Math::vec3d fn;
	};

	struct mesh {
		std::vector<triangle> tris;
	};

	/*
		Calculates the normal of the given triangle

		@param triangle - The given triangle

		Returns the resulting normal
	*/
	static Math::vec3d triangle_getNormal(triangle tri);
};
