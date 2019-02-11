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
	float vafan;
	vec3 g_CameraFront;
};

layout(std140, binding = 1) uniform PerObject
{
	mat4 g_Model;
	vec4 g_Color;
	float g_HasTexture;
	float g_HasNormalMap;
};

void main()
{
	vec4 worldPos = g_Model * vec4(g_Position, 1.0);
	vec3 normal = (g_Model * vec4(g_Normal, 0.0f)).xyz;
	vec3 tangent = (g_Model * vec4(g_Tangent, 0.0f)).xyz;
	
	vs_out.Normal = normal;
	vs_out.Tangent = tangent;
	vs_out.Binormal = cross(vs_out.Normal, vs_out.Tangent);
	vs_out.TexCoords = g_TexCoords;

	gl_Position = g_ViewProjection * worldPos;
}