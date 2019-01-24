#version 420

layout(location = 0) out vec4 g_OutColor;
layout(location = 1) out vec4 g_Normal;

in VS_OUT
{
	vec3 Normal;
	vec2 TexCoords;
} fs_in;

layout(binding = 0) uniform g_Texture;

layout(binding = 2) uniform FSPerObject
{
	vec4 g_Color;
	float g_HasTexture;
};

void main()
{
	g_OutColor = (g_Color * g_HasTexture) + texture(g_Texture, fs_in.TexCoords);
	g_Normal = vec4(normalize(fs_in.Normal), gl_FragCoord.z);
}