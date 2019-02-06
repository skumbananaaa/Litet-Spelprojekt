#version 420

in VS_OUT
{
	vec3 Normal;
	vec3 Tangent;
	vec3 Binormal;
	vec2 TexCoords;
} fs_in;

layout(binding = 0) uniform sampler2D g_Texture;
layout(binding = 1) uniform sampler2D g_NormalMap;

layout(std140, binding = 1) uniform PerObject
{
	mat4 g_Model;
	vec4 g_Color;
	float g_HasTexture;
	float g_HasNormalMap;
};

out vec4 g_OutColor;

void main()
{
	//COLOR
	vec3 mappedColor = texture(g_Texture, fs_in.TexCoords).rgb * g_HasTexture;
	vec3 uniformColor = g_Color.rgb * (1.0f - g_HasTexture);
	g_OutColor = vec4(uniformColor + mappedColor, 1.0f);
}