#include "TestObject.h"

void TestObject::construct() {
	int it = 0;

	const int numbOfVertices = 8;
	this->numbOfVertices = numbOfVertices;

	const int arrayLength = numbOfVertices * 4 * 2 + numbOfVertices * 3;
	float vertices[arrayLength];

	vector<glm::vec4> verticesVec;

	verticesVec.push_back(glm::vec4(-0.5f, 0.5f, 0.5f, 1.0f));
	verticesVec.push_back(glm::vec4(-0.5f, 0.5f, -0.5f, 1.0f));
	verticesVec.push_back(glm::vec4(0.5f, 0.5f, -0.5f, 1.0f));
	verticesVec.push_back(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));

	verticesVec.push_back(glm::vec4(-0.5f, -0.5f, 0.5f, 1.0f));
	verticesVec.push_back(glm::vec4(0.5f, -0.5f, 0.5f, 1.0f));
	verticesVec.push_back(glm::vec4(0.5f, -0.5f, -0.5f, 1.0f));
	verticesVec.push_back(glm::vec4(-0.5f, -0.5f, -0.5f, 1.0f));

	it += 8 * 4;

	memcpy(vertices, &verticesVec[0], verticesVec.size() * sizeof(glm::vec4));

	// Kolory

	while(it < numbOfVertices * 4 * 2) {
		vertices[it++] = float(rand() % 255) / 255; //r
		vertices[it++] = float(rand() % 255) / 255; //g
		vertices[it++] = float(rand() % 255) / 255; //b
		vertices[it++] = 1.0f; //a
	}

	// Indeks œcian

	const int numbOfTriangles = 6 * 2;
	this->numbOfTriangles = numbOfTriangles;

	vector<glm::uvec3> triangleIndexes;

	triangleIndexes.push_back(glm::uvec3(0, 1, 2));
	triangleIndexes.push_back(glm::uvec3(2, 3, 0));
	triangleIndexes.push_back(glm::uvec3(4, 5, 6));
	triangleIndexes.push_back(glm::uvec3(6, 7, 4));
	triangleIndexes.push_back(glm::uvec3(4, 0, 3));
	triangleIndexes.push_back(glm::uvec3(4, 3, 5));
	triangleIndexes.push_back(glm::uvec3(0, 4, 7));
	triangleIndexes.push_back(glm::uvec3(7, 1, 0));
	triangleIndexes.push_back(glm::uvec3(2, 1, 7));
	triangleIndexes.push_back(glm::uvec3(2, 7, 6));
	triangleIndexes.push_back(glm::uvec3(5, 3, 2));
	triangleIndexes.push_back(glm::uvec3(5, 2, 6));

	glGenBuffers(1, &triangleIndexBuffer);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangleIndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, triangleIndexes.size() * sizeof(glm::uvec3), &triangleIndexes[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// Normalne
	vector<glm::vec3> normals = getNormals(verticesVec, triangleIndexes);
	memcpy(&vertices[it], &normals[0], normals.size() * sizeof(glm::vec3));

	// Kopiowanie do grafiki

	glGenBuffers(1, &vectorBufferObject);

	glBindBuffer(GL_ARRAY_BUFFER, vectorBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// vao stuff

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vectorBufferObject);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *)(numbOfVertices * 4 * 4));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid *)(numbOfVertices * 4 * 2 * 4));


	glBindVertexArray(0);

}
void TestObject::render() {
	glBindVertexArray(vao);
	glDrawArrays(GL_POINTS, 0, numbOfVertices);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangleIndexBuffer);
	glDrawElements(GL_TRIANGLES, numbOfTriangles * 3, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}