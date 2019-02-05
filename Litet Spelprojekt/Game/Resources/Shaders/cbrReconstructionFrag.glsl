#version 420

layout(location = 0) out vec4 g_OutColor;

in VS_OUT
{
	vec2 TexCoords;
} fs_in;

layout(binding = 0) uniform sampler2D g_Color;
layout(binding = 1) uniform sampler2D g_Depth;
layout(binding = 2) uniform sampler2DMS g_ForColor;
layout(binding = 3) uniform sampler2DMS g_ForDepth;

void main()
{
	ivec2 size = textureSize(g_Color, 0);
	ivec2 frameSize = ivec2(size.x, size.y * 2);
	ivec2 frameCoord = ivec2(frameSize * fs_in.TexCoords);
	ivec2 modFrameCoord = frameCoord % 2;

	ivec2 sampleCoord = ivec2(frameCoord.x, (frameCoord.y - (frameCoord.x % 2)) / 2);
	ivec2 forSampleCoord = ivec2(sampleCoord.x / 2, sampleCoord.y);
	int subsample = sampleCoord.x % 2;

	float d1 = texelFetch(g_Depth, sampleCoord, 0).r;
	float d2 = texelFetch(g_ForDepth, forSampleCoord, subsample).r;

	vec4 color = vec4(0.0f);
	if (modFrameCoord.x == modFrameCoord.y)
	{
		if (d1 <= d2)
		{
			color = texelFetch(g_Color, sampleCoord, 0);
		}
		else
		{
			color = texelFetch(g_ForColor, forSampleCoord, subsample);
		}
	}
	else
	{
		if (d1 <= d2)
		{
			color = 
				texelFetch(g_Color, sampleCoord, 0) +
				texelFetch(g_Color, sampleCoord + ivec2(-1, 0), 0) +
				texelFetch(g_Color, sampleCoord + ivec2(1, 0), 0) +
				texelFetch(g_Color, sampleCoord + ivec2(0, 1), 0);
			color = color / 4.0f;
		}
		else
		{
			color = vec4(0.0f, 0.0f, 0.0f, 1.0f);
		}
	}

	g_OutColor = color;
}