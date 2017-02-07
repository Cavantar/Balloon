#version 330

uniform sampler2D uTexture;

//uniform samplerCube uTexture;
//smooth in vec3 eyeDirection;

out vec4 fragmentColor;

void main() {
    fragmentColor = texture(uTexture, vec2(0.5,0.5));
	//fragmentColor = texture(uTexture, eyeDirection);
	//fragmentColor = vec4(1.0, 0, 0, 1.0);
}