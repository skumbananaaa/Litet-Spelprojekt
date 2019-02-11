#pragma once
#include <EnginePch.h>
#include <Graphics/Geometry/AnimatedMesh.h>



glm::mat4 AnimatedMesh::AssimpToGLMMat4(aiMatrix4x4 mat)
{
	glm::mat4 retMat;

	retMat[0][0] = mat.a1;
	retMat[0][1] = mat.b1;
	retMat[0][2] = mat.c1;
	retMat[0][3] = mat.d1;

	retMat[1][0] = mat.a2;
	retMat[1][1] = mat.b2;
	retMat[1][2] = mat.c2;
	retMat[1][3] = mat.d2;

	retMat[2][0] = mat.a3;
	retMat[2][1] = mat.b3;
	retMat[2][2] = mat.c3;
	retMat[2][3] = mat.d3;

	retMat[3][0] = mat.a4;
	retMat[3][1] = mat.b4;
	retMat[3][2] = mat.c4;
	retMat[3][3] = mat.d4;

	return retMat;
}

glm::mat4 AnimatedMesh::GetJoinTransofrm()
{
	glm::mat4* pJointMat = new glm::mat4[m_NumOfJoints];
	

	return glm::mat4();
}

void AnimatedMesh::AddJointsToArray(Joint headJoint, glm::mat4* jointMatrix)
{
	for (int i = 0; i < headJoint.GetNrOfChildren(); i++)
	{
		jointMatrix[i] = headJoint.GetTransformMat();

	}

}
