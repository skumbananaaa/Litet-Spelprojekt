#if defined(FRAGMENT_SHADER)
layout(location = 0) out vec4 g_OutColor;

in VS_OUT
{
	vec2 TexCoords;
} fs_in;

layout(binding = 0) uniform sampler2D g_Color;

void main()
{
	//vec2 size = vec2(1.0f) / vec2(textureSize(g_Color, 0));
//	g_OutColor = 
//				(texture(g_Color, fs_in.TexCoords - vec2(size.x, size.y)) * 0.0625f) +
//				(texture(g_Color, fs_in.TexCoords + vec2(size.x, -size.y)) * 0.0625f) +
//				(texture(g_Color, fs_in.TexCoords - vec2(0, size.y)) * 0.125f)		 + 
//				(texture(g_Color, fs_in.TexCoords - vec2(size.x, 0)) * 0.125f)		 + 
//				(texture(g_Color, fs_in.TexCoords) * 0.25f)							 + 
//				(texture(g_Color, fs_in.TexCoords + vec2(size.x, 0)) * 0.125f)		 + 
//				(texture(g_Color, fs_in.TexCoords + vec2(0, size.y)) * 0.125f)		 +
//				(texture(g_Color, fs_in.TexCoords + vec2(-size.x, size.y)) * 0.0625f) +
//				(texture(g_Color, fs_in.TexCoords + vec2(size.x, size.y)) * 0.0625f);

	g_OutColor = texture(g_Color, fs_in.TexCoords);
}
#endif