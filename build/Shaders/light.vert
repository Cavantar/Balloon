#version 330

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec3 normal;

layout(std140) uniform GlobalMatrices
{
    mat4 cameraToClipMatrix;
    mat4 worldToCameraMatrix;
	vec3 dirToLight;
};
uniform mat4 localToWorldMatrix;

smooth out vec4 theColor;

void main()
{
    gl_Position = cameraToClipMatrix * (worldToCameraMatrix * (localToWorldMatrix * position));
	
	mat3 normalModelToCameraMatrixTemp = mat3( worldToCameraMatrix[0].xyz, worldToCameraMatrix[1].xyz, worldToCameraMatrix[2].xyz);
	mat3 localMatrix = mat3(localToWorldMatrix[0].xyz, localToWorldMatrix[1].xyz, localToWorldMatrix[2].xyz);
	
	vec3 normCamSpace = normalize(normalModelToCameraMatrixTemp * (localMatrix * normal));
	
	float cosAngIncidence = dot(normCamSpace, dirToLight);
	cosAngIncidence = clamp(cosAngIncidence, 0, 1);
	
	float ambientLight = 0.25f;
	theColor = color * cosAngIncidence + color * ambientLight;
}