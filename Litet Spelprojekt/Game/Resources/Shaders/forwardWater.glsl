layout(std140, binding = 0) uniform CameraBuffer
{
	mat4 g_ProjectionView;
	mat4 g_View;
	mat4 g_Projection;
	mat4 g_InverseView;
	mat4 g_InverseProjection;
	vec3 g_CameraLookAt;
	float pad1;
	vec3 g_CameraPosition;
};

#if defined(VERTEX_SHADER)
layout(location = 0) in vec3 g_Position;
layout(location = 1) in vec3 g_Normal;
layout(location = 2) in vec3 g_Tangent;
layout(location = 3) in vec2 g_TexCoords;
layout(location = 4) in mat4 g_InstanceModel;

out VS_OUT
{
	vec3 Position;
	vec3 Normal;
	vec2 TexCoords;
	vec4 ClipSpacePosition;
} vs_out;

const float tiling = 4.0f;
void main()
{
	vec4 worldPos = g_InstanceModel * vec4(g_Position, 1.0);
	vec3 normal = (g_InstanceModel * vec4(g_Normal, 0.0f)).xyz;

	vs_out.Position = worldPos.xyz;
	vs_out.Normal = normal;
	vs_out.TexCoords = tiling * g_TexCoords;
	vs_out.ClipSpacePosition = g_ProjectionView * worldPos;

	gl_Position = vs_out.ClipSpacePosition;
}

#elif defined(FRAGMENT_SHADER)
layout(early_fragment_tests) in;

layout(location = 0) out vec4 g_OutColor;

layout(binding = 1) uniform sampler2D normalMap;
layout(binding = 3) uniform sampler2D dudvMap;
layout(binding = 4) uniform sampler2D reflectionTexture;
layout(binding = 5) uniform sampler2D depthMap;

layout(std140, binding = 3) uniform WaterBuffer
{
	float g_DistortionFactor;
};

in VS_OUT
{
	vec3 Position;
	vec3 Normal;
	vec2 TexCoords;
	vec4 ClipSpacePosition;
} fs_in;

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

void main()
{
	//vec3 lightDir = normalize(vec3(0.0, 1.0, 0.5));
	//vec3 lightColor = vec3(1.0, 1.0, 1.0);
	//vec3 refractionColor = vec3(0.09, 0.34, 0.49); //Ocean Blue
	vec3 refractionColor = vec3(0.08, 0.07, 0.39); //Dark Ocean Blue

	vec2 ndcTexCoords = (fs_in.ClipSpacePosition.xy / fs_in.ClipSpacePosition.w) / 2.0 + 0.5;

	vec2 reflectionTexCoords = vec2(ndcTexCoords.x, -ndcTexCoords.y);
	vec2 depthTexCoords = vec2(ndcTexCoords.x, ndcTexCoords.y);

	float near = 0.1;
	float far = 100.0; //FIX THESE AS UNIFORMS
	float depth = texture(depthMap, depthTexCoords).r;
	float groundDistance = 2.0 * near * far / (far + near - (2.0 * depth - 1.0) * (far - near));
	depth = gl_FragCoord.z;
	float waterDistance = 2.0 * near * far / (far + near - (2.0 * depth - 1.0) * (far - near));
	float waterDepth = groundDistance - waterDistance;

	vec2 distortionTexCoords = texture(dudvMap, vec2(fs_in.TexCoords.x + g_DistortionFactor, fs_in.TexCoords.y)).rg * 0.1;
	distortionTexCoords = fs_in.TexCoords + vec2(distortionTexCoords.x, distortionTexCoords.y + g_DistortionFactor);
	vec2 totalDistortion = (texture(dudvMap, distortionTexCoords).rg * 2.0 - 1.0) * distortionStrength * clamp(waterDepth / depthOfFullDistortion, 0.0, 1.0);

	reflectionTexCoords += totalDistortion;
	reflectionTexCoords.x = clamp(reflectionTexCoords.x, 0.001, 0.999); //Fixar wobbly kant
	reflectionTexCoords.y = clamp(reflectionTexCoords.y, -0.999, -0.001);

	vec3 reflectionColor = texture(reflectionTexture, reflectionTexCoords).rgb;

	//NORMAL
	vec3 normal = texture(normalMap, distortionTexCoords).xyz;
	normal = normalize(vec3(normal.x * 2.0 - 1.0, normal.y * normalYSmoothness, normal.z * 2.0 - 1.0));

	vec3 viewDir = g_CameraPosition - fs_in.Position;
	float distFromCamera = length(viewDir);
	viewDir = normalize(viewDir);
	float refractionFactor = pow(max(dot(viewDir, normal), 0.0f), refractionExp);

	//Specular
	//vec3 halfwayDir = normalize(lightDir + viewDir);
	//float spec = pow(max(dot(normal, halfwayDir), 0.0), shininess);
	//vec3 specular = specularStrength * spec * lightColor * clamp(waterDepth / depthOfFullSpecular, 0.0, 1.0);

	//Fog
	//float visibility = clamp(exp(-pow(distFromCamera * fogDensity, fogGradient)), 0.0, 1.0);

	vec4 col = vec4(mix(reflectionColor, refractionColor, refractionFactor), 1.0);
	col = mix(col, vec4(0.7, 0.25, 0.33, 1.0), 0.25);// + vec4(specular, 0.0);
	//FragColor = mix(vec4(0.392, 0.584, 0.929, 1.0), FragColor, visibility); //Fog
	//FragColor.a = clamp(waterDepth / depthOfFullOpaque, 0.0, 1.0);
	g_OutColor = col;
}
#endif