#if defined(FRAGMENT_SHADER)
layout(location = 0) out vec4 g_OutColor;

layout(binding = 0) uniform sampler2D g_Color;

in VS_OUT
{
	vec2 TexCoords;
} fs_in;

void main()
{
	ivec2 size = textureSize(g_Color, 0);
	ivec2 frameSize = ivec2(size.x, size.y * 2);
	ivec2 frameCoord = ivec2(frameSize * fs_in.TexCoords);
	ivec2 sampleCoord = ivec2(frameCoord.x, (frameCoord.y - (frameCoord.x % 2)) / 2);

	vec4 color = vec4(0.0f);
	if (frameCoord.x % 2 == frameCoord.y % 2)
	{
		color = texelFetch(g_Color, sampleCoord, 0);
	}
	else
	{
		color = 
			texelFetch(g_Color, sampleCoord,				0) +
			texelFetch(g_Color, sampleCoord + ivec2(-1, 0), 0) +
			texelFetch(g_Color, sampleCoord + ivec2(1, 0),	0) +
			texelFetch(g_Color, sampleCoord + ivec2(0, 1),	0);
		color = color / 4.0f;
	}

	g_OutColor = color;
}
#endif