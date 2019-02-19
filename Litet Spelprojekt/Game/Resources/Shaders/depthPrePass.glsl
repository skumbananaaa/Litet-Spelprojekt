#if defined(VERTEX_SHADER)
layout(location = 0) in vec3 g_Position;
layout(location = 1) in vec3 g_Normal;
layout(location = 2) in vec3 g_Tangent;
layout(location = 3) in vec2 g_TexCoords;
layout(location = 4) in mat4 g_InstanceModel;

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

void main()
{
	vec4 position = g_InstanceModel * vec4(g_Position, 1.0);
	gl_ClipDistance[0] = dot(position, g_ClipPlane);
	gl_Position = g_ProjectionView * position;
}

#elif defined(FRAGMENT_SHADER)
void main()
{
}

#endif