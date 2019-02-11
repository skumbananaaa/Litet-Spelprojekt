#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <Graphics/Geometry/ImportAnimation.h>


AnimatedMesh* ImportAnimation::CreateAnimatedMeshFromColladaFile(const char* file)
{
	Assimp::Importer importer;

	const aiScene* pScene = importer.ReadFile(file, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_LimitBoneWeights | aiProcess_ConvertToLeftHanded);

	bool ret = false;
	
		m_GlobalInverseTransform = AssimpToGLMMat4(pScene->mRootNode->mTransformation);
		m_GlobalInverseTransform = glm::inverse(m_GlobalInverseTransform);

}

glm::mat4 ImportAnimation::AssimpToGLMMat4(aiMatrix4x4 mat)
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

	return glm::mat4();
}
