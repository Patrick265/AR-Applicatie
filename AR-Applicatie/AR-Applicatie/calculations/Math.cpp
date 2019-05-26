#include <algorithm>

#include "Math.h"




Math::vec3d Math::vector_add(const vec3d &vec1, const vec3d &vec2)
{
	vec3d new_vec;
	new_vec.x = vec1.x + vec2.x;
	new_vec.y = vec1.y + vec2.y;
	new_vec.z = vec1.z + vec2.z;

	return new_vec;
}

Math::vec3d Math::vector_sub(const vec3d &vec1, const vec3d &vec2)
{
	vec3d new_vec;
	new_vec.x = vec1.x - vec2.x;
	new_vec.y = vec1.y - vec2.y;
	new_vec.z = vec1.z - vec2.z;

	return new_vec;
}

Math::vec3d Math::vector_getInverse(const vec3d & vec)
{
	vec3d inv_vec;

	inv_vec.x = vec.x * -1;
	inv_vec.y = vec.y * -1;
	inv_vec.z = vec.z * -1;

	return inv_vec;
}

Math::vec3d Math::matrix_multiplyVector(const mat4x4 &mat, const vec3d &vec)
{
	vec3d new_vec;
	new_vec.x = vec.x * mat.m[0][0] + vec.y * mat.m[1][0] + vec.z * mat.m[2][0] + vec.w * mat.m[3][0];
	new_vec.y = vec.x * mat.m[0][1] + vec.y * mat.m[1][1] + vec.z * mat.m[2][1] + vec.w * mat.m[3][1];
	new_vec.z = vec.x * mat.m[0][2] + vec.y * mat.m[1][2] + vec.z * mat.m[2][2] + vec.w * mat.m[3][2];
	new_vec.w = vec.x * mat.m[0][3] + vec.y * mat.m[1][3] + vec.z * mat.m[2][3] + vec.w * mat.m[3][3];

	return new_vec;
}

Math::mat4x4 Math::matrix_multiplyMatrix(const mat4x4 &mat1, const mat4x4 &mat2)
{
	mat4x4 new_mat;

	for (int row = 0; row < 4; row++) {
		for (int col = 0; col < 4; col++) {
			new_mat.m[row][col] = mat1.m[row][0] * mat2.m[0][col] +
				mat1.m[row][1] * mat2.m[1][col] +
				mat1.m[row][2] * mat2.m[2][col] +
				mat1.m[row][3] * mat2.m[3][col];
		}
	}

	return new_mat;
}

Math::vec3d Math::normalize(const vec3d &v)
{
	vec3d new_vec;
	float l = sqrtf(v.x*v.x + v.y*v.y + v.z*v.z);

	new_vec.x = v.x / l;
	new_vec.y = v.y / l;
	new_vec.z = v.z / l;

	return new_vec;
}

float Math::dotProduct(const vec3d &v1, const vec3d &v2)
{
	return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
}

Math::vec3d Math::crossProduct(const vec3d &v1, const vec3d &v2)
{
	vec3d new_vec;
	new_vec.x = v1.y*v2.z - v1.z*v2.y;
	new_vec.y = v1.z*v2.x - v1.x*v2.z;
	new_vec.z = v1.x*v2.y - v1.y*v2.x;

	return new_vec;
}
