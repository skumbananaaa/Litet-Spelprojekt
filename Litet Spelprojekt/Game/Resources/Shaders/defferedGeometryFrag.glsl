#version 420

layout(location = 0) out vec4 g_OutColor;
layout(location = 1) out vec4 g_Normal;

in VS_OUT
{
	vec3 Normal;
	vec3 Tangent;
	vec3 Binormal;
	vec2 TexCoords;
} fs_in;

layout(binding = 0) uniform sampler2D g_Texture;
layout(binding = 1) uniform sampler2D g_NormalMap;

layout(binding = 2) uniform FSPerObject
{
	vec4 g_Color;
	float g_HasTexture;
	float g_HasNormalMap;
};

void main()
{
	g_OutColor = (g_Color * (1.0f - g_HasTexture)) + (texture(g_Texture, fs_in.TexCoords) * g_HasTexture);
	
	vec3 mappedNormal = (texture(g_NormalMap, fs_in.TexCoords).xyz * 2.0f) - vec3(1.0f);
	mappedNormal = (mappedNormal.r * fs_in.Tangent) + (mappedNormal.g * fs_in.Binormal) + (mappedNormal.b * fs_in.Normal); 

	vec3 normal = (fs_in.Normal * (1.0f - g_HasNormalMap)) + (mappedNormal * g_HasNormalMap);
	g_Normal = vec4(normalize(normal), gl_FragCoord.z);
}