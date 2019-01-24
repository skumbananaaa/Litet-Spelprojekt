#version 420

layout(location = 0) in vec3 g_Position;
layout(location = 1) in vec3 g_Normal;
layout(location = 2) in vec3 g_Tangent;
layout(location = 3) in vec2 g_TexCoords;

layout(std140, binding = 0) uniform VSPerFrame
{
	mat4 g_ViewProjection;
	vec3 g_CameraPosition;
};

layout(std140, binding = 1) uniform VSPerObject
{
	mat4 g_Model;
};

void main()
{
	gl_Position = g_ViewProjection * g_Model * vec4(g_Position, 1.0);
}