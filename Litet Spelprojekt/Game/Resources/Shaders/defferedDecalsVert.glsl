#version 420

layout(location = 0) in vec3 g_Position;
layout(location = 1) in vec3 g_Normal;
layout(location = 2) in vec3 g_Tangent;
layout(location = 3) in vec2 g_TexCoords;

layout(std140, binding = 0) uniform PerFrame
{
	mat4 g_ViewProjection;
	mat4 g_InverseView;
	mat4 g_InverseProjection;
};

layout(std140, binding = 1) uniform PerObject
{
	mat4 g_Model;
	mat4 g_InverseModel;
};

out VS_OUT
{
	vec4 PositionClipSpace;
} vs_out;

void main()
{
	vs_out.PositionClipSpace = g_ViewProjection * g_Model * vec4(g_Position, 1.0f);
	gl_Position = vs_out.PositionClipSpace;
}