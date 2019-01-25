#version 420

layout(location = 0) in vec3 g_Position;
layout(location = 1) in vec3 g_Normal;
layout(location = 2) in vec3 g_Tangent;
layout(location = 3) in vec2 g_TexCoords;

out VS_OUT
{
	vec3 Normal;
	vec3 Tangent;
	vec3 Binormal;
	vec2 TexCoords;
} vs_out;

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
	vec4 worldPos = g_Model * vec4(g_Position, 1.0);
	gl_ClipDistance[0] = dot(worldPos, vec4(0.0f, 1.0f, 0.0f, 0.01f));

	vec4 normal = g_Model * vec4(g_Normal, 0.0f);
	vec4 tangent = g_Model * vec4(g_Tangent, 0.0f);
	
	vs_out.Normal = normal.xyz;
	vs_out.Tangent = tangent.xyz;
	vs_out.Binormal = cross(normal.xyz, tangent.xyz);
	vs_out.TexCoords = g_TexCoords;

	gl_Position = g_ViewProjection * worldPos;
}