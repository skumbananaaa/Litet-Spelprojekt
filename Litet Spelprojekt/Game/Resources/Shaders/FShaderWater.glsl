
in VS_OUT
{
	vec3 Position;
	vec3 Normal;
	vec2 TexCoords;
	vec4 ClipSpacePosition;
} fs_in;

layout (std140, binding = 1) uniform PerFrameBlock
{
	mat4 cameraCombined;
	vec3 cameraPosition;
	float distortionMoveFactor;
};

layout (binding = 0) uniform sampler2D reflectionTexture;
layout (binding = 1) uniform sampler2D dudvMap;
layout (binding = 2) uniform sampler2D normalMap;
layout (binding = 3) uniform sampler2DMS depthMap;

out vec4 FragColor;

const float distortionStrength = 0.005;
const float specularStrength = 0.6;
const float shininess = 20.0;
const float refractionExp = 1.5;
const float normalYSmoothness = 4.0;
const float depthOfFullOpaque = 0.7; 
const float depthOfFullDistortion = 0.9;
const float depthOfFullSpecular = 0.7;

const float fogDensity = 0.075;
const float fogGradient = 5.0;

float SampleDepth(vec2 texCoords)
{
	ivec2 size = textureSize(depthMap);
	ivec2 coords = ivec2(size * texCoords);

	float depth = 1.0f;
	for (int i = 0; i < 2; i++)
	{
		depth = min(depth, texelFetch(depthMap, coords, i).r);
	}

	return depth;
}

void main()
{
	vec3 lightDir = normalize(vec3(0.0, 1.0, 0.5));
	vec3 lightColor = vec3(1.0, 1.0, 1.0);
	//vec3 refractionColor = vec3(0.09, 0.34, 0.49); //Ocean Blue
	vec3 refractionColor = vec3(0.08, 0.07, 0.39); //Dark Ocean Blue


	vec2 ndcTexCoords = (fs_in.ClipSpacePosition.xy / fs_in.ClipSpacePosition.w) / 2.0 + 0.5;

	vec2 reflectionTexCoords = vec2(ndcTexCoords.x, -ndcTexCoords.y);
	vec2 depthTexCoords = vec2(ndcTexCoords.x, ndcTexCoords.y);

	float near = 0.1;
	float far = 100.0; //FIX THESE AS UNIFORMS
	float depth = SampleDepth(depthTexCoords);
	float groundDistance = 2.0 * near * far / (far + near - (2.0 * depth - 1.0) * (far - near));
	depth = gl_FragCoord.z;
	float waterDistance = 2.0 * near * far / (far + near - (2.0 * depth - 1.0) * (far - near));
	float waterDepth = groundDistance - waterDistance;

	vec2 distortionTexCoords = texture(dudvMap, vec2(fs_in.TexCoords.x + distortionMoveFactor, fs_in.TexCoords.y)).rg * 0.1;
	distortionTexCoords = fs_in.TexCoords + vec2(distortionTexCoords.x, distortionTexCoords.y + distortionMoveFactor);
	vec2 totalDistortion = (texture(dudvMap, distortionTexCoords).rg * 2.0 - 1.0) * distortionStrength * clamp(waterDepth / depthOfFullDistortion, 0.0, 1.0);
	
	reflectionTexCoords += totalDistortion;
	reflectionTexCoords.x = clamp(reflectionTexCoords.x, 0.001, 0.999); //Fixar wobbly kant
	reflectionTexCoords.y = clamp(reflectionTexCoords.y, -0.999, -0.001);

	vec3 reflectionColor = texture(reflectionTexture, reflectionTexCoords).rgb;

	vec3 normal = texture(normalMap, distortionTexCoords).xyz;
	normal = normalize(vec3(normal.x * 2.0 - 1.0, normal.y * normalYSmoothness, normal.z * 2.0 - 1.0));

	vec3 viewDir = cameraPosition - fs_in.Position;
	float distFromCamera = length(viewDir);
	viewDir = normalize(viewDir); 
	float refractionFactor = pow(dot(viewDir, normal), refractionExp);

	//Specular
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(normal, halfwayDir), 0.0), shininess);
	vec3 specular = specularStrength * spec * lightColor * clamp(waterDepth / depthOfFullSpecular, 0.0, 1.0);

	//Fog
	float visibility = clamp(exp(-pow(distFromCamera * fogDensity, fogGradient)), 0.0, 1.0);

	FragColor = vec4(mix(reflectionColor, refractionColor, refractionFactor), 1.0);
	FragColor = mix(FragColor, vec4(0.7, 0.25, 0.33, 1.0), 0.25) + vec4(specular, 0.0);
	//FragColor = mix(vec4(0.392, 0.584, 0.929, 1.0), FragColor, visibility); //Fog
	FragColor.a = clamp(waterDepth / depthOfFullOpaque, 0.0, 1.0);
}