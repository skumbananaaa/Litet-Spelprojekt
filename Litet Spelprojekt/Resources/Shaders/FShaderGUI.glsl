
in vec2 TexCoords;
in vec4 Color;

out vec4 FragColor;

layout(binding = 0) uniform sampler2D textureToSample;

layout (std140, binding = 0) uniform PerObjectBlock
{
	mat4 projection;
};

void main()
{    
    FragColor = texture(textureToSample, TexCoords) * Color;
}