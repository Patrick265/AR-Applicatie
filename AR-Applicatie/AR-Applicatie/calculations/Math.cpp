#include <algorithm>
#include "Math.h"

Math::vec3d Math::vectorAdd(const vec3d &vec1, const vec3d &vec2)
{
	vec3d newVec;
	newVec.x = vec1.x + vec2.x;
	newVec.y = vec1.y + vec2.y;
	newVec.z = vec1.z + vec2.z;

	return newVec;
}

Math::vec3d Math::vectorSub(const vec3d &vec1, const vec3d &vec2)
{
	vec3d newVec;
	newVec.x = vec1.x - vec2.x;
	newVec.y = vec1.y - vec2.y;
	newVec.z = vec1.z - vec2.z;

	return newVec;
}

Math::vec3d Math::vectorGetInverse(const vec3d & vec)
{
	vec3d invVec;

	invVec.x = vec.x * -1;
	invVec.y = vec.y * -1;
	invVec.z = vec.z * -1;

	return invVec;
}

Math::vec3d Math::matrixMultiplyVector(const mat4x4 &mat, const vec3d &vec)
{
	vec3d newVec;
	newVec.x = vec.x * mat.m[0][0] + vec.y * mat.m[1][0] + vec.z * mat.m[2][0] + vec.w * mat.m[3][0];
	newVec.y = vec.x * mat.m[0][1] + vec.y * mat.m[1][1] + vec.z * mat.m[2][1] + vec.w * mat.m[3][1];
	newVec.z = vec.x * mat.m[0][2] + vec.y * mat.m[1][2] + vec.z * mat.m[2][2] + vec.w * mat.m[3][2];
	newVec.w = vec.x * mat.m[0][3] + vec.y * mat.m[1][3] + vec.z * mat.m[2][3] + vec.w * mat.m[3][3];

	return newVec;
}

Math::mat4x4 Math::matrixMultiplyMatrix(const mat4x4 &mat1, const mat4x4 &mat2)
{
	mat4x4 newMat;

	for (auto row = 0; row < 4; row++)
	{
		for (auto col = 0; col < 4; col++)
		{
			newMat.m[row][col] = mat1.m[row][0] * mat2.m[0][col] +
				mat1.m[row][1] * mat2.m[1][col] +
				mat1.m[row][2] * mat2.m[2][col] +
				mat1.m[row][3] * mat2.m[3][col];
		}
	}

	return newMat;
}

Math::vec3d Math::normalize(const vec3d &v)
{
	vec3d newVec;
	const auto l = sqrtf(v.x*v.x + v.y*v.y + v.z*v.z);

	newVec.x = v.x / l;
	newVec.y = v.y / l;
	newVec.z = v.z / l;

	return newVec;
}

float Math::dotProduct(const vec3d &v1, const vec3d &v2)
{
	return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
}

Math::vec3d Math::crossProduct(const vec3d &v1, const vec3d &v2)
{
	vec3d newVec;
	newVec.x = v1.y*v2.z - v1.z*v2.y;
	newVec.y = v1.z*v2.x - v1.x*v2.z;
	newVec.z = v1.x*v2.y - v1.y*v2.x;

	return newVec;
}
