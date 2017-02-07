#include "Basket.h"

void Basket::construct(float balloonRadius, float lowestHeight) {
	numbOfVertices = 8; // Kosz
	numbOfVertices += 4; // Linki
	numbOfCircleSegments = 8;
	numbOfVertices += numbOfCircleSegments * 2; // *2 because upper are also needed

	arraySize = numbOfVertices * 4 * 2 + numbOfVertices * 3;


	float balloonOffset = 2.5f;

	vector<float> rawData;
	rawData.resize(arraySize);
	
	//Wierzcho³ki

	vector<glm::vec4> vertices;
	vertices.resize(numbOfVertices);

	float scale = 0.25f;

	// Góra

	vertices[0] = glm::vec4(-1.0f * scale, 1.0f * scale, 1.0f * scale, 1.0f);
	vertices[1] = glm::vec4(1.0f * scale, 1.0f * scale, 1.0f * scale, 1.0f);
	vertices[2] = glm::vec4(1.0f * scale, 1.0f * scale, -1.0f * scale, 1.0f);
	vertices[3] = glm::vec4(-1.0f * scale, 1.0f * scale, -1.0f * scale, 1.0f);

	// Dó³

	vertices[4] = glm::vec4(-1.0f * scale, -1.0f * scale, 1.0f * scale, 1.0f);
	vertices[5] = glm::vec4(1.0f * scale, -1.0f * scale, 1.0f * scale, 1.0f);
	vertices[6] = glm::vec4(1.0f * scale, -1.0f * scale, -1.0f * scale, 1.0f);
	vertices[7] = glm::vec4(-1.0f * scale, -1.0f * scale, -1.0f * scale, 1.0f);

	// Linki

	scale = 0.1f;
	float heightOffset = 0.5f;

	vertices[8] = glm::vec4(-1.0f * scale, 1.0f * scale + heightOffset, 1.0f * scale, 1.0f);
	vertices[9] = glm::vec4(1.0f * scale, 1.0f * scale + heightOffset, 1.0f * scale, 1.0f);
	vertices[10] = glm::vec4(1.0f * scale, 1.0f * scale + heightOffset, -1.0f * scale, 1.0f);
	vertices[11] = glm::vec4(-1.0f * scale, 1.0f * scale + heightOffset, -1.0f * scale, 1.0f);

	// Circle
	for(int i = 0; i < numbOfCircleSegments; i++) {
		glm::vec4 tempVector(0, 0, 0, 1.0f);
		tempVector.y = vertices[11].y;
		tempVector.x = (float)sin(2 * 3.14 * ((float)i / numbOfCircleSegments)) * 0.5f * pow(scale, 0.5f);
		tempVector.z = (float)cos(2 * 3.14 * ((float)i / numbOfCircleSegments)) * 0.5f * pow(scale, 0.5f) * 0.9f;
		vertices[i + 12] = tempVector;
	}
	for(int i = 0; i < numbOfCircleSegments; i++) {
		glm::vec4 tempVector(0, 0, 0, 1.0f);
		tempVector.y = lowestHeight + balloonOffset;
		tempVector.x = (float)sin(2 * 3.14 * ((float)i / numbOfCircleSegments)) * balloonRadius;
		tempVector.z = (float)cos(2 * 3.14 * ((float)i / numbOfCircleSegments)) * balloonRadius;
		vertices[i + 12 + numbOfCircleSegments] = tempVector;
	}

	// -------

	translateVec4(vertices, glm::vec4(0, -balloonOffset, 0, 0));
	memcpy(&rawData[0], &vertices[0], vertices.size() * sizeof(glm::vec4));

	// Kolory
	vector<glm::vec4> colors;
	colors.resize(numbOfVertices);
	for(int i = 0; i < numbOfVertices; i++) colors[i] = glm::vec4(199.0f / 255.0f, 155.0f / 255.0f, 34.0f / 255.0f, 1.0f);
	memcpy(&rawData[vertices.size() * 4], &colors[0], colors.size() * sizeof(glm::vec4));

	// Trójk¹ty
	vector<glm::uvec3> triangles;

	addIndexedQuad(triangles, glm::uvec4(0, 1, 5, 4));
	addIndexedQuad(triangles, glm::uvec4(3, 0, 4, 7));
	addIndexedQuad(triangles, glm::uvec4(2, 3, 7, 6));
	addIndexedQuad(triangles, glm::uvec4(1, 2, 6, 5));
	addIndexedQuad(triangles, glm::uvec4(4, 5, 6, 7));

	numbOfTriangles = triangles.size();

	glGenBuffers(1, &triangleIndexBuffer);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangleIndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, triangles.size() * sizeof(glm::uvec3), &triangles[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// Lines 

	vector<glm::uvec2> lines;

	lines.push_back(glm::uvec2(0, 8));
	lines.push_back(glm::uvec2(1, 9));
	lines.push_back(glm::uvec2(3, 11));
	lines.push_back(glm::uvec2(2, 10));

	for(int i = 0; i < numbOfCircleSegments; i++) {
		lines.push_back(glm::uvec2(i + 12, i + 12 + numbOfCircleSegments));
	}
	numbOfLines = lines.size();

	glGenBuffers(1, &addLineIndex);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, addLineIndex);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, lines.size() * sizeof(glm::uvec2), &lines[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// Circle Lines

	vector<unsigned int> circleIndices;
	for(int i = 0; i < numbOfCircleSegments; i++) {
		circleIndices.push_back(i + 12);
	}

	glGenBuffers(1, &circleLineIndex);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, circleLineIndex);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, circleIndices.size() * sizeof(unsigned int), &circleIndices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// Normalne
	vector<glm::vec3> normals = getNormals(vertices, triangles);
	memcpy(&rawData[vertices.size() * 4 + colors.size() * 4], &normals[0], normals.size() * sizeof(glm::vec3));

	// Odwrotne Trójk¹ty
	reverseTriangleIndexes(triangles);
	glGenBuffers(1, &revTriangleIndexBuffer);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, revTriangleIndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, triangles.size() * sizeof(glm::uvec3), &triangles[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


	// Kopiowanie do grafiki
	glGenBuffers(1, &bufferObject);

	glBindBuffer(GL_ARRAY_BUFFER, bufferObject);
	glBufferData(GL_ARRAY_BUFFER, arraySize * 4, &rawData[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Tworzenie VAO

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, bufferObject);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *)(numbOfVertices * 4 * 4));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid *)(numbOfVertices * 4 * 2 * 4));

	glBindVertexArray(0);
}
void Basket::render(int renderType) {
	glBindVertexArray(vao);
	glDrawArrays(GL_POINTS, 0, numbOfVertices);

	// Linki
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, addLineIndex);
	glDrawElements(GL_LINES, numbOfLines * 2, GL_UNSIGNED_INT, 0);
	// Ko³owe Linki
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, circleLineIndex);
	glDrawElements(GL_LINE_LOOP, numbOfCircleSegments, GL_UNSIGNED_INT, 0);


	switch(renderType) {
		case 0:
			glDrawArrays(GL_POINTS, 0, numbOfVertices);
			break;
		case 1:
			/*glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, groundLineIndex);
			glDrawElements(GL_LINES, numbOfLines * 2, GL_UNSIGNED_INT, 0);
			*/
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangleIndexBuffer);
			glDrawElements(GL_TRIANGLES, numbOfTriangles * 3, GL_UNSIGNED_INT, 0);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, revTriangleIndexBuffer);
			glDrawElements(GL_TRIANGLES, numbOfTriangles * 3, GL_UNSIGNED_INT, 0);
			break;
		case 2:;
			/*glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, groundLineIndex);
			glDrawElements(GL_LINES, numbOfLines * 2, GL_UNSIGNED_INT, 0);
			break;*/
	}
	glBindVertexArray(0);
}