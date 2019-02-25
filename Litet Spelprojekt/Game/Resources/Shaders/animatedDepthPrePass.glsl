#define MAX_BONES 60

#if defined(VERTEX_SHADER)
layout(location = 0) in vec3 g_Position;
layout(location = 1) in vec3 g_Normal;
layout(location = 2) in vec3 g_Tangent;
layout(location = 3) in vec2 g_TexCoords;
layout(location = 4) in ivec4 g_BonesIDs;
layout(location = 5) in vec4 g_Weights;

layout(std140, binding = 0) uniform CameraBuffer
{
	mat4 g_ProjectionView;
	mat4 g_View;
	mat4 g_Projection;
	mat4 g_InverseView;
	mat4 g_InverseProjection;
	vec3 g_CameraLookAt;
	float pad1;
	vec3 g_CameraPosition;
};

layout(std140, binding = 4) uniform PlaneBuffer
{
	vec4 g_ClipPlane;
};

layout(std140, binding = 7) uniform BoneBuffer
{
	mat4 g_Bones[MAX_BONES];
	mat4 g_Model;
};

void main()
{
	mat4 BoneTransform = g_Bones[g_BonesIDs[0]] * g_Weights[0];
	BoneTransform += g_Bones[g_BonesIDs[1]] * g_Weights[1];
	BoneTransform += g_Bones[g_BonesIDs[2]] * g_Weights[2];
	BoneTransform += g_Bones[g_BonesIDs[3]] * g_Weights[3];

	mat4 finalModel = g_Model * BoneTransform;
	vec4 worldPos = finalModel * vec4(g_Position, 1.0);

	gl_ClipDistance[0] = dot(worldPos, g_ClipPlane);

	gl_Position = g_ProjectionView * worldPos;
}

#elif defined(FRAGMENT_SHADER)

void main()
{
}

#endif