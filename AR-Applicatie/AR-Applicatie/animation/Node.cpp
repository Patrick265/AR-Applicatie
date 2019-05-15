#include "gl/freeglut.h"

#include "Node.h"

Node::Node(Math::vec3d pos, Math::vec3d rot)
{
	this->name = "centre";
	this->mesh = {};
	this->texture_id = -1;

	this->pos = pos;
	this->rot = rot;
}

Node::Node(std::string name, Graphics::mesh mesh, uint16_t texture_id, Math::vec3d pos, Math::vec3d rot)
{
	this->name = name;
	this->mesh = mesh;
	this->texture_id = texture_id;

	this->pos = pos;
	this->rot = rot;
}

Node::~Node() {
	
	for (std::vector< Node* >::iterator it = children.begin(); it != children.end(); ++it)
	{
		delete (*it);
	}
}

void drawMesh(Graphics::mesh mesh, uint16_t texture_id)
{
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_TRIANGLES);

	glColor3f(1.0f, 1.0f, 1.0f);

	for (Graphics::triangle tri : mesh.tris) {
		
		Math::vec3d normal = Graphics::triangle_getNormal(tri);
		glNormal3f(normal.x, normal.y, normal.z);
		for (int i = 0; i < 3; i++)
		{
			glTexCoord2f(tri.vt[i].x, tri.vt[i].y);
			glVertex3f(tri.p[i].x, tri.p[i].y, tri.p[i].z);
		}
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void Node::draw()
{
	glPushMatrix();	

	glTranslatef(pos.x, pos.y, pos.z);	
	
	glRotatef(rot.x, 1, 0, 0);
	glRotatef(rot.y, 0, 1, 0);
	glRotatef(rot.z, 0, 0, 1);	

	drawMesh(mesh, texture_id);
	
	for (Node* child : children) {
		child->draw();
	}

	glPopMatrix();
}

std::string Node::getName()
{
	return name;
}

void Node::setRotation(Math::vec3d rot)
{
	this->rot = rot;
}

void Node::setPosition(Math::vec3d pos)
{
	this->pos = pos;
}

std::vector<Node*> Node::getChildren()
{
	return children;
}

void Node::addChild(Node* child)
{
	children.push_back(child);

}
