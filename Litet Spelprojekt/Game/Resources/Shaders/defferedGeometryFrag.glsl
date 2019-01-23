#version 420

layout(location = 0) out vec4 g_Color;
layout(location = 1) out vec4 g_Normal;

in VS_OUT
{
	vec3 Normal;
} fs_in;

void main()
{
	g_Color = vec4(vec3(1.0f, 0.0f, 1.0f), 1.0f);
	g_Normal = vec4(normalize(fs_in.Normal), 1.0f);
}