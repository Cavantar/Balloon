#version 330
layout(location = 0) in vec4 aPosition;

//uniform mat4 uProjectionMatrix;
//uniform mat4 uWorldToCameraMatrix;

//smooth out vec3 eyeDirection;

void main() {
    //mat4 inverseProjection = inverse(uProjectionMatrix);
    //mat3 inverseModelview = transpose(mat3(uWorldToCameraMatrix));
    //vec3 unprojected = (inverseProjection * aPosition).xyz;
    //eyeDirection = inverseModelview * unprojected;
	//eyeDirection = vec3(1.0,0,0);
    gl_Position = aPosition;
} 