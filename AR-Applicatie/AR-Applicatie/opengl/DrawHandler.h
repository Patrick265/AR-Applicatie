#pragma once

#include <vector>

#include "../calculations/Graphics.h"

class DrawHandler {
private:
public:
	/*
		Draws the mesh and gives it a texture

		@param mesh - The given mesh
		@param texture_id - The given texture

	*/
	static void drawMesh(const Graphics::mesh &mesh, const uint16_t &texture_id);
};