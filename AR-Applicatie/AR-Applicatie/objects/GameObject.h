#pragma once

#include "../calculations/Graphics.h"

class GameObject {

protected:

	//x, y, z are the coordinates of the object
	Math::vec3d position;

	//x, y, z are the rotations of the x, y and z axis
	Math::vec3d rotation;

	//x, y, z are the scaling factors of the x, y and z axis
	Math::vec3d scale;

	Graphics::mesh mesh;
	uint16_t texture_id;

public:
	
	/*
		Constructor with mesh and id
		
		@param mesh - The mesh of the gameobject
		@param texture_id - The texture id of the gameobject
	*/
	GameObject(Graphics::mesh mesh, uint16_t texture_id);

	/*
		Sets the position of the gameobject

		@param pos - The given position
	*/
	void setPosition(Math::vec3d pos);

	/*
		Sets the rotation of the gameobject

		@param rot - The given rotation
	*/
	void setRotation(Math::vec3d rot);

	/*
		Sets the scale of the gameobject

		@param scale - The given scale
	*/
	void setScale(Math::vec3d scale);

	/*
		Gets the position of the gameobject

		Returns the position
	*/
	Math::vec3d getPosition();

	/*
		Gets the rotation of the gameobject

		Returns the rotation
	*/
	Math::vec3d getRotation();

	/*
		Gets the scale of the gameobject

		Returns the scale
	*/
	Math::vec3d getScale();

	/*
		Gets the mesh of the gameobject

		Returns the mesh
	*/
	Graphics::mesh getMesh();

	/*
		Gets the texture id of the gameobject

		Returns the texture id
	*/
	uint16_t getTextureId();
};