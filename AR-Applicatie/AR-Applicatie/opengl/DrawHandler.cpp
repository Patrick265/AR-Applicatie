#include "DrawHandler.h"
#include "GL/freeglut.h"

void DrawHandler::drawMesh(const Graphics::mesh &mesh, const uint16_t &textureId)
{
	glBindTexture(GL_TEXTURE_2D, textureId);
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_TRIANGLES);

	glColor3f(1.0f, 1.0f, 1.0f);

	for (const auto vertex : mesh.vertices)
	{
		glNormal3f(vertex.fn.x, vertex.fn.y, vertex.fn.z);
		glTexCoord2f(vertex.vt.x, vertex.vt.y);
		glVertex3f(vertex.p.x, vertex.p.y, vertex.p.z);
	}

	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void DrawHandler::drawMeshArray(const Graphics::mesh &mesh, const uint16_t &textureId)
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glColor3f(1.0f, 1.0f, 1.0f);

	//The amount of elements in the vec3d struct.
	const uint8_t containerSize = sizeof(Math::vec3d) / sizeof(float);

	glTexCoordPointer(2, GL_FLOAT, sizeof(Graphics::vertex), (float*)mesh.vertices.data() + containerSize * 2);
	glNormalPointer(GL_FLOAT, sizeof(Graphics::vertex), (float*)mesh.vertices.data() + containerSize);
	glVertexPointer(3, GL_FLOAT, sizeof(Graphics::vertex), (float*)mesh.vertices.data());

	glBindTexture(GL_TEXTURE_2D, textureId);
	glEnable(GL_TEXTURE_2D);

	glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(mesh.vertices.size()));
	glDisable(GL_TEXTURE_2D);

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}
