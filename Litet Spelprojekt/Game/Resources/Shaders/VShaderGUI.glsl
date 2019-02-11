
layout (location = 0) in vec2 position;
layout (location = 1) in vec2 texCoords;
layout (location = 2) in vec4 color;

out vec2 TexCoords;
out vec4 Color;

layout (std140, binding = 0) uniform PerObjectBlock
{
	mat4 projection;
};

void main()
{
    gl_Position = projection * vec4(position, 0.0, 1.0);
    
	vec2 uv = texCoords;
	uv.y = 1.0f - uv.y;
	TexCoords = uv;

	Color = color;
}  