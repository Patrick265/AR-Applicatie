#include "Graphics.h"

Math::vec3d Graphics::triangle_getNormal(triangle tri)
{
	Math::vec3d line1 = Math::vector_sub(tri.p[0], tri.p[1]);
	Math::vec3d line2 = Math::vector_sub(tri.p[0], tri.p[2]);
	Math::vec3d normal = Math::crossProduct(line1, line2);
	normal = Math::normalize(normal);
	return normal;
}
