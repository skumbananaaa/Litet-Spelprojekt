#version 420

out VS_OUT
{
	vec2 TexCoords;
} vs_out;

void main()
{
	float x = -1.0 + float((gl_VertexID & 1) << 2);
	float y = -1.0 + float((gl_VertexID & 2) << 1);

	vs_out.TexCoords.x = (x + 1.0)*0.5;
	vs_out.TexCoords.y = (y + 1.0)*0.5;
	
	gl_Position = vec4(x, y, 0, 1);
}