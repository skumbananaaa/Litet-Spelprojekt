#version 420 core

in VS_OUT
{
	vec3 Position;
	vec3 Normal;
} fs_in;

layout (std140, binding = 1) uniform PerFrameBlock
{
	mat4 cameraCombined;
	vec3 cameraPosition;
};

out vec4 FragColor;

void main()
{
	vec3 lightDir = normalize(vec3(0.0, 1.0, 0.5));
	vec3 lightColor = vec3(1.0, 1.0, 1.0);

	vec3 norm = normalize(fs_in.Normal);

	//AMBIENT
	float ambientStrength = 0.3;
	vec3 ambient = ambientStrength * lightColor;

	//DIFFUSE
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	//SPECULAR
	float specularStrength = 1.0;
	vec3 viewDir = normalize(cameraPosition - fs_in.Position);
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(norm, halfwayDir), 0.0), 16.0);
	vec3 specular = specularStrength * spec * lightColor;

	FragColor = vec4((ambient + diffuse + specular) * vec3(1.0f, 0.0f, 1.0f), 1.0);
	//FragColor = vec4(-norm, 1.0);
}