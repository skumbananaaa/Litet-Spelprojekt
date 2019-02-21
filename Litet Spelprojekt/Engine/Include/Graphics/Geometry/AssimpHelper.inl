#pragma once

inline glm::mat4 AssimpToGLMMat4(const aiMatrix4x4& mat)
{
	glm::mat4 retMat;

	retMat[0][0] = mat[0][0];
	retMat[0][1] = mat[1][0];
	retMat[0][2] = mat[2][0];
	retMat[0][3] = mat[3][0];

	retMat[1][0] = mat[0][1];
	retMat[1][1] = mat[1][1];
	retMat[1][2] = mat[2][1];
	retMat[1][3] = mat[3][1];

	retMat[2][0] = mat[0][2];
	retMat[2][1] = mat[1][2];
	retMat[2][2] = mat[2][2];
	retMat[2][3] = mat[3][2];

	retMat[3][0] = mat[0][3];
	retMat[3][1] = mat[1][3];
	retMat[3][2] = mat[2][3];
	retMat[3][3] = mat[3][3];

	return retMat;
}

inline glm::mat4 AssimpScaleMat4(aiVector3D& vec)
{
	glm::mat4 retMat;

	retMat[0][0] = vec.x; retMat[0][1] = 0.0f;   retMat[0][2] = 0.0f;   retMat[0][3] = 0.0f;
	retMat[1][0] = 0.0f;   retMat[1][1] = vec.y; retMat[1][2] = 0.0f;   retMat[1][3] = 0.0f;
	retMat[2][0] = 0.0f;   retMat[2][1] = 0.0f;   retMat[2][2] = vec.z; retMat[2][3] = 0.0f;
	retMat[3][0] = 0.0f;   retMat[3][1] = 0.0f;   retMat[3][2] = 0.0f;   retMat[3][3] = 1.0f;

	return retMat;
}

inline glm::mat4 AssimpTranslateMat4(aiVector3D& vec)
{
	glm::mat4 retMat;
	retMat[0][0] = 1.0f; retMat[1][0] = 0.0f; retMat[2][0] = 0.0f; retMat[3][0] = vec.x;
	retMat[0][1] = 0.0f; retMat[1][1] = 1.0f; retMat[2][1] = 0.0f; retMat[3][1] = vec.y;
	retMat[0][2] = 0.0f; retMat[1][2] = 0.0f; retMat[2][2] = 1.0f; retMat[3][2] = vec.z;
	retMat[0][3] = 0.0f; retMat[1][3] = 0.0f; retMat[2][3] = 0.0f; retMat[3][3] = 1.0f;

	return retMat;
}