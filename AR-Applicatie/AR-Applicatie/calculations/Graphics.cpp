#include "Graphics.h"

Math::vec3d Graphics::getNormal(const Math::vec3d & vec1, const Math::vec3d & vec2, const Math::vec3d & vec3)
{
	const auto line1 = Math::vectorSub(vec1, vec2);
	const auto line2 = Math::vectorSub(vec1, vec3);
	auto normal = Math::crossProduct(line1, line2);
	normal = Math::normalize(normal);
	return normal;
}

void Graphics::inverseNormals(mesh & mesh)
{
	for (auto& vertex : mesh.vertices)
	{
		vertex.fn = Math::vectorGetInverse(vertex.fn);
		vertex.vn = Math::vectorGetInverse(vertex.vn);
	}
}
