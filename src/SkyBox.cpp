#include "SkyBox.h"
#include "ImageData.h"
//TODO Rysowanie œladów
void SkyBox::construct() {
	loadTexture("Gfx\\skybox.bmp");
	
	vector<glm::vec4> verticesVec(4);
	float distance = 1.0f;
	float length = 0.75f;
	verticesVec[0] = glm::vec4(-length, length, distance, 1.0f);
	verticesVec[1] = glm::vec4( length, length, distance, 1.0f);
	verticesVec[2] = glm::vec4( length, -length, distance, 1.0f);
	verticesVec[3] = glm::vec4(-length, -length, distance, 1.0f);

	vector<glm::uvec3> trianglesVec(2);
	trianglesVec[0] = glm::uvec3(0, 1, 3);
	trianglesVec[1] = glm::uvec3(1, 2, 3);


	glGenBuffers(1, &triangleIndexBuffer);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangleIndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, trianglesVec.size() * sizeof(glm::uvec3), &trianglesVec[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// Kopiowanie do grafiki

	glGenBuffers(1, &verticesBuffer);

	glBindBuffer(GL_ARRAY_BUFFER, verticesBuffer);
	glBufferData(GL_ARRAY_BUFFER, verticesVec.size() * sizeof(glm::vec4), &verticesVec[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Vertex Array Buffer

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, verticesBuffer);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

	glBindVertexArray(0);

	doShaderInit();
}
void SkyBox::render() {
	glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_CUBE_MAP, cubeTextureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	
	glUseProgram(skyBoxProgram);
	glBindVertexArray(vao);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangleIndexBuffer);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glUseProgram(0);
}
void SkyBox::doShaderInit() {
	vector<GLuint> shaderList;

	shaderList.push_back(CreateShader(GL_VERTEX_SHADER, "Shaders\\SkyBox\\vertex.vert"));
	shaderList.push_back(CreateShader(GL_FRAGMENT_SHADER, "Shaders\\SkyBox\\fragment.frag"));

	skyBoxProgram = CreateProgram(shaderList);
}
void SkyBox::loadTexture(string filename){
	textureId = loadBMP_custom(filename.c_str());

	glGenTextures(1, &cubeTextureId);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeTextureId);

	ImageData up(filename);
	
	for(int i = 0; i < 6; i++) 
		up.applyTexture();

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}
void SkyBox::createUniforms() {
	glUseProgram(skyBoxProgram);

	uProjectionMatrix = glGetUniformLocation(skyBoxProgram, "uProjectionMatrix");
	
	glUseProgram(0);
}
void SkyBox::setPerspMat(glutil::MatrixStack matrix) {


}