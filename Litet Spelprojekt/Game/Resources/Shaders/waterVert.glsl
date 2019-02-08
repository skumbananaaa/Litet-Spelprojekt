#version 420

#define NUM_CLIP_DISTANCES 8

layout(location = 0) in vec3 g_Position;
layout(location = 1) in vec3 g_Normal;
layout(location = 2) in vec3 g_Tangent;
layout(location = 3) in vec2 g_TexCoords;
layout(location = 4) in mat4 g_InstanceModel;

out VS_OUT
{
	vec3 Position;
	vec3 Normal;
	vec2 TexCoords;
	vec4 ClipSpacePosition;
} vs_out;

layout(std140, binding = 0) uniform CameraBuffer
{
	mat4 g_ProjectionView;
	mat4 g_View;
	mat4 g_Projection;
	mat4 g_InverseView;
	mat4 g_InverseProjection;
	vec3 g_CameraPosition;
};

void main()
{
	vec4 worldPos = g_InstanceModel * vec4(g_Position, 1.0);
	vec3 normal = (g_InstanceModel * vec4(g_Normal, 0.0f)).xyz;
	
	vs_out.Position = worldPos.xyz;
	vs_out.Normal = normal;
	vs_out.TexCoords = g_TexCoords;
	vs_out.ClipSpacePosition = g_ProjectionView * worldPos;

	gl_Position = vs_out.ClipSpacePosition;
}