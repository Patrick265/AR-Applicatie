#pragma once

class TextureHandler
{
public:
	/*
		Reads a texture from an image, and adds it to opengl

		@param image_path - The relative path of the image
		@param amount_of_textures - The amount of textures that already exist

		Returns the id of the texture
	*/
	static uint16_t addTexture(const std::string &image_path, const size_t &amount_of_textures);
};
