#include "DrawHandler.h"

#include "GL/freeglut.h"


void DrawHandler::drawMesh(const Graphics::mesh &mesh, const uint16_t &texture_id)
{
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_TRIANGLES);

	glColor3f(1.0f, 1.0f, 1.0f);

	for (Graphics::vertex vert : mesh.vertices) {

		glNormal3f(vert.fn.x, vert.fn.y, vert.fn.z);
		glTexCoord2f(vert.vt.x, vert.vt.y);
		glVertex3f(vert.p.x, vert.p.y, vert.p.z);
	}

	glEnd();
	glDisable(GL_TEXTURE_2D);
}


void DrawHandler::drawMesh_array(const Graphics::mesh &mesh, const uint16_t &texture_id)
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glColor3f(1.0f, 1.0f, 1.0f);

	//The amount of elements in the vec3d struct.
	uint8_t container_size = sizeof(Math::vec3d) / sizeof(float);

	glTexCoordPointer(2, GL_FLOAT, sizeof(Graphics::vertex), ((float*)mesh.vertices.data()) + container_size * 2);
	glNormalPointer(GL_FLOAT, sizeof(Graphics::vertex), ((float*)mesh.vertices.data()) + container_size);
	glVertexPointer(3, GL_FLOAT, sizeof(Graphics::vertex), ((float*)mesh.vertices.data()));

	glBindTexture(GL_TEXTURE_2D, texture_id);
	glEnable(GL_TEXTURE_2D);

	glDrawArrays(GL_TRIANGLES, 0, mesh.vertices.size());
	glDisable(GL_TEXTURE_2D);

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}
