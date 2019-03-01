#if defined(VERTEX_SHADER)
layout(location = 0) in vec3 g_Position;
layout(location = 1) in vec3 g_Normal;
layout(location = 2) in vec2 g_TexCoords;
layout(location = 3) in mat4 g_InstanceModel;

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

layout(std140, binding = 1) uniform ModelBuffer
{
	mat4 g_Model;
};

out vec4 g_FragPosition;

void main()
{
	g_FragPosition = g_ProjectionView * g_Model * vec4(g_Position, 1.0f);
	gl_Position = g_FragPosition;
}

#elif defined(FRAGMENT_SHADER)
layout(early_fragment_tests) in;

in vec4 g_FragPosition;

layout(std140, binding = 2) uniform ModelBuffer
{
	vec3 g_LightPosition;
	float g_FarPlane;
};

void main()
{
	float dist = length(g_FragPosition.xyz - g_LightPosition);
	gl_FragDepth = dist / g_FarPlane;
}
#endif