#pragma once

#include "../calculations/Graphics.h"

class ObjLoader {
private:
public:
	/*
		Loads the obj file and extracts the triangles which form the mesh

		@param filename - The name of the file

		Returns the extracted mesh
	*/
	static Graphics::mesh loadObj(const std::string filename);
};