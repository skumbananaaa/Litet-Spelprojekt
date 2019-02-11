
in vec3 texCoords;
out vec4 FragColor;

uniform samplerCube skyBox;

void main()
{
	FragColor = texture(skyBox, texCoords);
}