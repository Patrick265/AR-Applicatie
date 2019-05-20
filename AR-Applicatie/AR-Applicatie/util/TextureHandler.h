#pragma once


class TextureHandler {

private:

public:

	/*
		Adds a texture to the

		@param image_path - The relative path of the image
		@param amount_of_textures - The amount of textures that already exist

		Returns the id of the texture
	*/
	static uint16_t addTexture(const std::string &image_path, const int &amount_of_textures);

};