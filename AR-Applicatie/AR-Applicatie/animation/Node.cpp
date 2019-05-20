#include "gl/freeglut.h"

#include "Node.h"
#include "../opengl/DrawHandler.h"
#include "../calculations/Graphics.h"

Node::Node()
{
	this->name = "centre";
	this->mesh = "none";
	this->texture = "none";

	this->pos = Math::vec3d{ 0,0,0 };
	this->rot = Math::vec3d{ 0,0,0 };
}

Node::Node(Math::vec3d pos, Math::vec3d rot)
{
	this->name = "centre";
	this->mesh = "none";
	this->texture = "none";

	this->pos = pos;
	this->rot = rot;
}

Node::Node(const std::string &name, const std::string &mesh, const std::string &texture, const Math::vec3d &pos, const Math::vec3d &rot)
	:name(name), mesh(mesh), texture(texture), pos(pos), rot(rot)
{
}

Node::Node(const Node & node)
	: name(node.name), pos(node.pos), rot(node.rot), children(node.children), mesh(node.mesh), texture(node.texture)
{
}

Node::~Node() {
}

void Node::draw(std::map<std::string, Node>&nodes, const std::map<std::string, Graphics::mesh> &meshes, const std::map<std::string, uint16_t> &textures)
{
	glPushMatrix();

	glTranslatef(pos.x, pos.y, pos.z);

	glRotatef(rot.x, 1, 0, 0);
	glRotatef(rot.y, 0, 1, 0);
	glRotatef(rot.z, 0, 0, 1);

	DrawHandler::drawMesh(meshes.at(mesh), textures.at(texture));

	for (std::string child : children) {
		nodes.at(child).draw(nodes, meshes, textures);
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

std::vector<std::string> Node::getChildren()
{
	return children;
}

void Node::addChild(std::string child)
{
	children.push_back(child);
}
