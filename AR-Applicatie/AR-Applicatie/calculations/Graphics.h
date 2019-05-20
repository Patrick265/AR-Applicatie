#pragma once

#include <vector>

#include "Math.h"

class Graphics {
private:
public:

	struct vertex {
		//vertex position
		Math::vec3d p;
		//Face normal
		Math::vec3d fn;
		//texture coordinate
		Math::vec2d vt;
		//vertex normal
		Math::vec3d vn;
	};

	struct mesh {
		std::vector<vertex> vertices;
	};

	/*
		Calculates the normal of the given three vertices (triangle)

		@param vec1 - The first vertex
		@param vec2 - The second vertex
		@param vec3 - The third vertex

		Returns the resulting normal
	*/
	static Math::vec3d getNormal(const Math::vec3d &vec1, const Math::vec3d &vec2, const Math::vec3d &vec3);
};
