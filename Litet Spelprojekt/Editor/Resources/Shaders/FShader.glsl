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
	float padding;
	vec4 clipPlane;
};

out vec4 FragColor;

const float fogDensity = 0.075;
const float fogGradient = 5.0;

void main()
{
	vec3 lightDir = normalize(vec3(0.0, 1.0, 0.5));
	vec3 lightColor = vec3(1.0, 1.0, 1.0);

	vec3 norm = normalize(fs_in.Normal);

	//AMBIENT
	float ambientStrength = 0.3;
	vec3 ambient = ambientStrength * lightColor;

	//DIFFUSE
	float diffuseStrength = 0.5;
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = vec3(0.0);
	vec3 specular = vec3(0.0);

	vec3 viewDir = cameraPosition - fs_in.Position;
	float distFromCamera = length(viewDir);
	viewDir = normalize(cameraPosition - fs_in.Position);

	if (diff > 0.0)
	{
		diffuse = diffuseStrength * diff * lightColor;

		//SPECULAR
		float specularStrength = 0.6;
		vec3 halfwayDir = normalize(lightDir + viewDir);
		float spec = pow(max(dot(norm, halfwayDir), 0.0), 256.0);
		specular = specularStrength * spec * lightColor;
	}

	//Fog
	float visibility = clamp(exp(-pow(distFromCamera * fogDensity, fogGradient)), 0.0, 1.0);

	FragColor = vec4(clamp((ambient + diffuse) * vec3(1.0f, 0.0f, 1.0f) + specular, 0.0, 1.0), 1.0);
	//FragColor = mix(vec4(0.392, 0.584, 0.929, 1.0), FragColor, visibility); //Fog
	//FragColor = vec4(-norm, 1.0);
}