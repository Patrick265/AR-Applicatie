#pragma once

#include <vector>
#include "../calculations/Graphics.h"

class Node {

private:	
	std::string name;

	Math::vec3d pos;
	Math::vec3d rot;

	std::vector<Node *> children;

	Graphics::mesh mesh;
	uint16_t texture_id;
	
public:
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
	Node(std::string name, Graphics::mesh mesh, uint16_t texture_id, Math::vec3d pos, Math::vec3d rot);
	~Node();

	/*
		Draws the Node
	*/
	void draw();

	/*
		Adds a child to the Node
		
		@param child - The given child
	*/
	void addChild(Node* child);
	
	/*
		Returns the name of the Node
	*/
	std::string getName();

	/*
		Returns the children of the Node
	*/
	std::vector<Node *> getChildren();

	/*
		Sets the rotation of the Node
	*/
	void setRotation(Math::vec3d rot);
};
