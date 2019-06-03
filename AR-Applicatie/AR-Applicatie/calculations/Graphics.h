#ifndef GRAPHICS
#define GRAPHICS

#include <vector>
#include "Math.h"

class Graphics
{
public:
	struct vertex
	{
		//vertex position
		Math::vec3d p;
		//vertex normal
		Math::vec3d vn;
		//texture coordinate
		Math::vec2d vt;
		//Face normal
		Math::vec3d fn;
	};

	struct mesh
	{
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

	/*
		Inverses all the normals in the mesh

		@param mesh - The given mesh
	*/
	static void inverseNormals(mesh &mesh);
};
#endif // !GRAPHICS