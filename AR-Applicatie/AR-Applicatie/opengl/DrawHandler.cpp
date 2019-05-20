#include "DrawHandler.h"

#include "GL/freeglut.h"


void DrawHandler::drawMesh(const Graphics::mesh & mesh, const uint16_t & texture_id)
{
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_TRIANGLES);

	glColor3f(1.0f, 1.0f, 1.0f);

	for (Graphics::triangle tri : mesh.tris) {

		glNormal3f(tri.fn.x, tri.fn.y, tri.fn.z);
		for (int i = 0; i < 3; i++)
		{
			glTexCoord2f(tri.vt[i].x, tri.vt[i].y);
			glVertex3f(tri.p[i].x, tri.p[i].y, tri.p[i].z);
		}
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);

}
