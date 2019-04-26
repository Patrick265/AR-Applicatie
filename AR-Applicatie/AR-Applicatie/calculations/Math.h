#pragma once

class Math {

private:


public:

	struct vec2d {
		float x = 0;
		float y = 0;
	};

	struct vec3d {
		float x = 0;
		float y = 0;
		float z = 0;
		float w = 1;
	};

	struct mat4x4 {
		float m[4][4] = { 0 };
	};


	/*
		Adds two vectors together

		@param vec1 - The first vector
		@param vec2 - The second vector

		Returns the resulting vector
	*/
	static vec3d vector_add(vec3d vec1, vec3d vec2);

	/*
		Subtracts two vectors

		@param vec1 - The first vector
		@param vec2 - The second vector

		Returns the resulting vector
	*/
	static vec3d vector_sub(vec3d vec1, vec3d vec2);

	/*
		Multiplies a vector with a matrix

		@param mat - The matrix
		@param vec - The vector

		Returns the resulting vector
	*/
	static vec3d matrix_multiplyVector(mat4x4 mat, vec3d vec);


	/*
		Multiplies a matrix with a matrix

		@param mat1 - The first matrix
		@param mat2 - The second matrix

		Returns the resulting matrix
	*/
	static mat4x4 matrix_multiplyMatrix(mat4x4 mat1, mat4x4 mat2);

	/*
		Normalizes the vector

		@param v - The vector

		Returns the normalized vector
	*/
	static vec3d normalize(vec3d v);

	/*
		Obtains the length of v2 projected onto v1

		@param v1 - The first vector
		@param v2 - The second vector

		Returns the dotproduct
	*/
	static float dotProduct(vec3d v1, vec3d v2);


	/*
	Obtains the normal vector

	@param v1 - The first vector
	@param v2 - The second vector

	Returns the normal vector
*/
	static vec3d crossProduct(vec3d v1, vec3d v2);
};