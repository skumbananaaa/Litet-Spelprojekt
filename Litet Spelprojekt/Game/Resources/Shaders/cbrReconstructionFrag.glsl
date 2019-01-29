#version 420

layout(location = 0) out vec4 g_OutColor;

in VS_OUT
{
	vec2 TexCoords;
} fs_in;

layout(binding = 0) uniform sampler2D g_Color;
layout(binding = 1) uniform sampler2D g_Depth;

void main()
{
	ivec2 size = textureSize(g_Color, 0);
	ivec2 coord = ivec2(size * fs_in.TexCoords);
	ivec2 frameSize = ivec2(size.x, size.y * 2);
	ivec2 frameCoord = ivec2(frameSize * fs_in.TexCoords);
	ivec2 modFrameCoord = frameCoord % 2;

	ivec2 sampleCoord = ivec2(coord.x, (frameCoord.y - (frameCoord.x % 2)) / 2);
	if (modFrameCoord.x == modFrameCoord.y)
	{
		g_OutColor = texelFetch(g_Color, sampleCoord, 0);
		gl_FragDepth = texelFetch(g_Depth, sampleCoord, 0).r;
	}
	else
	{
		g_OutColor = vec4(0.0f);
		gl_FragDepth = 0.0f;
	}
}