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


		vec3d& operator=(const vec3d& a)
		{
			x = a.x;
			y = a.y;
			z = a.z;

			return *this;
		}

		vec3d& operator+=(const vec3d& a)
		{
			x += a.x;
			y += a.y;
			z += a.z;

			return *this;
		}
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
	static vec3d vector_add(const vec3d &vec1, const vec3d &vec2);

	/*
		Subtracts two vectors

		@param vec1 - The first vector
		@param vec2 - The second vector

		Returns the resulting vector
	*/
	static vec3d vector_sub(const vec3d &vec1, const vec3d &vec2);

	static vec3d vector_getInverse(const vec3d &vec);

	/*
		Multiplies a vector with a matrix

		@param mat - The matrix
		@param vec - The vector

		Returns the resulting vector
	*/
	static vec3d matrix_multiplyVector(const mat4x4 &mat, const vec3d &vec);


	/*
		Multiplies a matrix with a matrix

		@param mat1 - The first matrix
		@param mat2 - The second matrix

		Returns the resulting matrix
	*/
	static mat4x4 matrix_multiplyMatrix(const mat4x4 &mat1, const mat4x4 &mat2);

	/*
		Normalizes the vector

		@param v - The vector

		Returns the normalized vector
	*/
	static vec3d normalize(const vec3d &v);

	/*
		Obtains the length of v2 projected onto v1

		@param v1 - The first vector
		@param v2 - The second vector

		Returns the dotproduct
	*/
	static float dotProduct(const vec3d &v1, const vec3d &v2);


	/*
		Obtains the normal vector of the two vectors

		@param v1 - The first vector
		@param v2 - The second vector

		Returns the normal vector
	*/
	static vec3d crossProduct(const vec3d &v1, const vec3d &v2);
};