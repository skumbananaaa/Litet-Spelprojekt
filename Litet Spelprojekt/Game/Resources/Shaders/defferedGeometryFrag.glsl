#version 420

layout(location = 0) out vec4 g_Color;
layout(location = 1) out vec4 g_Normal;
layout(location = 2) out vec4 g_Position;

in VS_OUT
{
	vec3 Normal;
	vec3 Position;
} fs_in;

void main()
{
	g_Color = vec4(vec3(1.0f, 0.0f, 1.0f), 1.0f);
	g_Normal = vec4(normalize(fs_in.Normal), 1.0f);
	g_Position = vec4(fs_in.Position, 1.0f);
}