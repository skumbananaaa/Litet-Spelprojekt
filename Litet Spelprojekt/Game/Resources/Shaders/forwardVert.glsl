
#define NUM_CLIP_DISTANCES 8

layout(location = 0) in vec3 g_Position;
layout(location = 1) in vec3 g_Normal;
layout(location = 2) in vec3 g_Tangent;
layout(location = 3) in vec2 g_TexCoords;
layout(location = 4) in mat4 g_InstanceModel;

out VS_OUT
{
	vec3 WorldPosition;
	vec3 Normal;
	vec3 Tangent;
	vec3 Binormal;
	vec2 TexCoords;
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

layout(std140, binding = 3) uniform PlaneBuffer
{
	vec4 g_ClipPlane;
};


void main()
{
	vec4 worldPos = g_InstanceModel * vec4(g_Position, 1.0);
	gl_ClipDistance[0] = dot(worldPos, g_ClipPlane);

	vec3 normal = (g_InstanceModel * vec4(g_Normal, 0.0f)).xyz;
	vec3 tangent = (g_InstanceModel * vec4(g_Tangent, 0.0f)).xyz;

	vs_out.WorldPosition = worldPos.xyz;
	vs_out.Normal = normal;
	vs_out.Tangent = tangent;
	vs_out.Binormal = cross(vs_out.Normal, vs_out.Tangent);
	vs_out.TexCoords = g_TexCoords;

	gl_Position = g_ProjectionView * worldPos;
}