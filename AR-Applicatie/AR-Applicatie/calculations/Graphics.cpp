#include "Graphics.h"


Math::vec3d Graphics::getNormal(const Math::vec3d & vec1, const Math::vec3d & vec2, const Math::vec3d & vec3)
{
	Math::vec3d line1 = Math::vector_sub(vec1, vec2);
	Math::vec3d line2 = Math::vector_sub(vec1, vec3);
	Math::vec3d normal = Math::crossProduct(line1, line2);
	normal = Math::normalize(normal);
	return normal;
}

void Graphics::inverseNormals(Graphics::mesh & mesh)
{
	for (int i = 0; i < mesh.vertices.size(); i++) 
	{
		mesh.vertices[i].fn = Math::vector_getInverse(mesh.vertices[i].fn);
		mesh.vertices[i].vn = Math::vector_getInverse(mesh.vertices[i].vn);
	}
}
