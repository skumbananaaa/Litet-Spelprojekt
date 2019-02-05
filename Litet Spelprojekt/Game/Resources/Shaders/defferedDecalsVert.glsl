#version 420

layout(location = 0) in vec3 g_Position;
layout(location = 1) in vec3 g_Normal;
layout(location = 2) in vec3 g_Tangent;
layout(location = 3) in vec2 g_TexCoords;
layout(location = 4) in mat4 g_InstanceModel;
layout(location = 8) in mat4 g_InstanceInverseModel;
layout(location = 12) in vec3 g_InstanceDirection;

layout(std140, binding = 0) uniform PerFrame
{
	mat4 g_ViewProjection;
	mat4 g_InverseView;
	mat4 g_InverseProjection;
};

out VS_OUT
{
	vec4 PositionClipSpace;
	mat4 InverseModel;
	vec3 Direction;
} vs_out;

void main()
{
	vs_out.Direction = g_InstanceDirection;
	vs_out.InverseModel = g_InstanceInverseModel;
	vs_out.PositionClipSpace = g_ViewProjection * g_InstanceModel * vec4(g_Position, 1.0f);

	gl_Position = vs_out.PositionClipSpace;
}