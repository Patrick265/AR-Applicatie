#pragma once

#include <map>
#include <vector>
#include "../calculations/Graphics.h"

class Node {

private:
	std::string name;

	Math::vec3d pos;
	Math::vec3d rot;

	//The names of the children, so they can be retrieved from the map in the rig
	std::vector<std::string> children;

	std::string mesh;
	std::string texture;

public:
	Node();
	/*
		The constructor for the centre Node
		It doesnt have parameters for the mesh and texture because the centre doesn't have visuals

		@param pos - The position of the Node
		@param rot - The rotation of the Node
	*/
	Node(Math::vec3d pos, Math::vec3d rot);

	/*
		The constructor for all other nodes

		@param name - The name of the Node
		@param mesh - The mesh (or object)
		@param texture_id - The id of the texture
		@param pos - The position of the Node
		@param rot - The rotation of the Node
	*/
	Node(const std::string &name, const std::string &mesh, const std::string &texture, const Math::vec3d &pos, const Math::vec3d &rot);
	Node(const Node &node);
	~Node();

	/*
		Draws the Node

		@param nodes - The map of nodes, used to find the child Nodes

	*/
	void draw(std::map<std::string, Node>& nodes, const std::map<std::string, Graphics::mesh> &meshes, const std::map<std::string, uint16_t> &textures);

	/*
		Adds a child to the Node

		@param child - The given child
	*/
	void addChild(std::string child);

	/*
		Returns the name of the Node
	*/
	std::string getName();

	/*
		Returns the children of the Node
	*/
	std::vector<std::string> getChildren();

	/*
		Sets the rotation of the Node

		@param rot - The given rotation
	*/
	void setRotation(Math::vec3d rot);

	/*
		Sets the position of the Node

		@param pos - The given position
	*/
	void setPosition(Math::vec3d pos);

	void setMesh(const std::string &mesh) { this->mesh = mesh; }
	void setTexture(const std::string &texture) { this->texture = texture; }

};
