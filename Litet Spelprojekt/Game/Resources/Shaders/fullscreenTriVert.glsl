#version 420

out VS_OUT
{
	vec2 TexCoords;
} vs_out;

void main()
{
	float x = -1.0f + float((gl_VertexID & 1) << 2);
	float y = -1.0f + float((gl_VertexID & 2) << 1);

	vs_out.TexCoords.x = (x + 1.0f) * 0.5f;
	vs_out.TexCoords.y = (y + 1.0f) * 0.5f;
	
	gl_Position = vec4(x, y, 0.0f, 1.0f);
}