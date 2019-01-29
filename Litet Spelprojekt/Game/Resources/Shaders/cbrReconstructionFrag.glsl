#version 420

layout(location = 0) out vec4 g_OutColor;

in VS_OUT
{
	vec2 TexCoords;
} fs_in;

layout(binding = 0) uniform sampler2D g_Color;
layout(binding = 1) uniform sampler2D g_Depth;
layout(binding = 2) uniform sampler2D g_LastColor;
layout(binding = 3) uniform sampler2D g_LastDepth;

void main()
{
	ivec2 size = textureSize(g_Color, 0);
	ivec2 coord = ivec2(size * fs_in.TexCoords);
	ivec2 frameSize = ivec2(size.x, size.y * 2);
	ivec2 frameCoord = ivec2(frameSize * fs_in.TexCoords);
	ivec2 modFrameCoord = frameCoord % 2;

	vec4 color = vec4(0.0f);
	float depth = 0.0f;
	if (modFrameCoord.x == modFrameCoord.y)
	{
		ivec2 sampleCoord = ivec2(coord.x, (frameCoord.y - (frameCoord.x % 2)) / 2);
		color = texelFetch(g_Color, sampleCoord, 0);
		depth = texelFetch(g_Depth, sampleCoord, 0).r;
	}
	else
	{
		ivec2 sampleCoord = ivec2(coord.x, (frameCoord.y - ((frameCoord.x + 1) % 2)) / 2);
		color = texelFetch(g_LastColor, sampleCoord, 0);
		depth = texelFetch(g_LastDepth, sampleCoord, 0).r;
	}

	g_OutColor = color;
	gl_FragDepth = depth;
}