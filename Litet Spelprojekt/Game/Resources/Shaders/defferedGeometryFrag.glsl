#version 420

layout(location = 0) out vec4 g_OutColor;
layout(location = 1) out vec4 g_Normal;

in VS_OUT
{
	vec3 Normal;
	vec2 TexCoords;
} fs_in;

layout(binding = 0) uniform sampler2D g_Texture;

layout(binding = 2) uniform FSPerObject
{
	vec4 g_Color;
	float g_HasTexture;
};

void main()
{
	g_OutColor = (g_Color * (1.0f - g_HasTexture)) + (texture(g_Texture, fs_in.TexCoords) * g_HasTexture);
	g_Normal = vec4(normalize(fs_in.Normal), gl_FragCoord.z);
}