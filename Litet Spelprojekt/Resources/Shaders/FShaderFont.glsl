
in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D text;

layout (std140, binding = 0) uniform PerFrameBlock
{
	mat4 projection;
    vec3 color;
};

void main()
{    
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);
    FragColor = vec4(color, 1.0) * sampled;
}