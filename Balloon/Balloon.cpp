#include "Balloon.h"

void Balloon::construct() {
	const int verticalLevels = 20;
	const int horizontalLevels = 20;
	const float radius = 1.0f;
	const float coneStart = 0.7f;
	const int indentsMod = 5;

	const int numberOfVertices = verticalLevels * horizontalLevels;
	this->numberOfVertices = numberOfVertices;
	const int balloonArraySize = numberOfVertices * 2 * 4 + numberOfVertices * 3;

	float balloonData[balloonArraySize];

	int it = 0;
	float tempRadius;
	float y = 0;
	float prevY;
	float coneDistance;

	float lowestRadius = 1000000.0f;
	float lowestY = 0;

	// Konstrukcja geometrii

	vector<glm::vec4> verticesVec;
	glm::vec4 tempVec;

	for(int i = 0; i < verticalLevels; i++) {
		tempVec.x = radius * (float)sin((3.14) * ((double)i / (double)verticalLevels));

		if(i / (float)verticalLevels < coneStart){
			tempRadius = tempVec.x;
			prevY = tempVec.y;
			tempVec.y = radius * (float)cos((3.14) * ((double)i / (double)verticalLevels))*0.95f;
			coneDistance = tempVec.y - prevY;
		}
		else {
			tempRadius *= 0.85f;
			tempVec.y += coneDistance * 1.2f;
			if(i == verticalLevels - 1) lowestRadius = tempRadius;
		}
		for(int j = 0 ; j < horizontalLevels; j++) {			
			tempVec.x = tempRadius * (float)sin((3.14 * 2) * ((double)j / (double)horizontalLevels));
			tempVec.z = tempRadius * (float)cos((3.14 * 2) * ((double)j / (double)horizontalLevels));

			if(indentsMod != 0)if(!(j % indentsMod)) {
				tempVec.x *= 0.9f;
				tempVec.z *= 0.9f;
			}
			tempVec.w = 1.0f;
			if(i == 0) {
				tempVec.x = 0;
				tempVec.z = 0;
			}
			verticesVec.push_back(tempVec);
			if(tempVec.y < lowestY) lowestY = tempVec.y;
		}
	}
	it = verticalLevels * horizontalLevels * 4;

	memcpy(balloonData, &verticesVec[0], verticesVec.size() * sizeof(glm::vec4));


	//// Kolory
	const float colors[] = {
		0.0f, 0.0f, 0.8f, 1.0f,
		0.0f, 0.6f, 0.0f, 1.0f
		, 0.5f, 0.0f, 0.0f, 0.5f
	};
	int colorIt = 0;
	int layerIndex = 0;

	while(it < numberOfVertices * 4 * 2) {
		balloonData[it++] = colors[colorIt * 4];
		balloonData[it++] = colors[colorIt * 4 + 1];
		balloonData[it++] = colors[colorIt * 4 + 2];
		balloonData[it++] = colors[colorIt * 4 + 3];

		colorIt++ ;
		colorIt = colorIt % (sizeof(colors) / (4 * 4));
	}

	// Tryby trójk¹tów i linii

	numbOfTriangles = horizontalLevels * (verticalLevels - 1) * 2;

	glm::uvec3 tempIndexVec;
	vector<glm::uvec3> triangles;

	for(int y = 0; y < verticalLevels - 1; y++) {
		for(int x = 0; x < horizontalLevels; x++) {
			tempIndexVec.x = x + y * horizontalLevels;
			tempIndexVec.y = (x + 1) % horizontalLevels + y * horizontalLevels;
			tempIndexVec.z = (x + 1) % horizontalLevels + (y + 1) * horizontalLevels;

			triangles.push_back(tempIndexVec);

			tempIndexVec.x = x + y * horizontalLevels;
			tempIndexVec.y = (x + 1) % horizontalLevels + (y + 1) * horizontalLevels;
			tempIndexVec.z = x + (y + 1) * horizontalLevels;

			triangles.push_back(tempIndexVec);
		}
	}

	glGenBuffers(1, &triangleIndexBuffer);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangleIndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, triangles.size() * sizeof(glm::uvec3), &triangles[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	
	AdjacencyList adjacencyList = createGridAdjacencyList(verticesVec, glm::uvec2(verticalLevels, horizontalLevels), true);
	vector<glm::vec3>& normals = getNormals(verticesVec, triangles, adjacencyList); // , adjacencyList
	memcpy(&balloonData[it], &normals[0], normals.size() * sizeof(glm::vec3));

	// Reverse Triangle Index Buffer
	reverseTriangleIndexes(triangles);

	glGenBuffers(1, &revTriangleIndexBuffer);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, revTriangleIndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, triangles.size() * sizeof(glm::uvec3), &triangles[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	

	// Kopiowanie do grafiki

	glGenBuffers(1, &balloonBufferObject);

	glBindBuffer(GL_ARRAY_BUFFER, balloonBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(balloonData), balloonData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	const int numbOfLines = (4 + (horizontalLevels - 2) * 3 + 2 +
							 (3 + (horizontalLevels - 2) * 2 + 1) * (verticalLevels - 1)) * 2;

	balloonNumberOfLineIndices = numbOfLines;
	GLushort lineIndex[numbOfLines];
	int lineIterator = 0;

	for(int x = 0; x < horizontalLevels; x++) {
		if(x == 0) {
			// Góra
			lineIndex[lineIterator++] = 0;
			lineIndex[lineIterator++] = 1;
			// Lewo
			lineIndex[lineIterator++] = 0;
			lineIndex[lineIterator++] = horizontalLevels;
			// Prawo
			lineIndex[lineIterator++] = 1;
			lineIndex[lineIterator++] = horizontalLevels + 1;
			// Dó³
			lineIndex[lineIterator++] = horizontalLevels;
			lineIndex[lineIterator++] = horizontalLevels + 1;
		}
		else {
			if(x == horizontalLevels - 1) {
				// Góra
				lineIndex[lineIterator++] = horizontalLevels - 1;
				lineIndex[lineIterator++] = 0;

				// Dó³

				lineIndex[lineIterator++] = (horizontalLevels * 2) - 1;
				lineIndex[lineIterator++] = horizontalLevels;

			}
			else {
				// Góra 
				lineIndex[lineIterator++] = x;
				lineIndex[lineIterator++] = x + 1;
				// Dó³
				lineIndex[lineIterator++] = horizontalLevels + x;
				lineIndex[lineIterator++] = horizontalLevels + x + 1;
				// Prawo
				lineIndex[lineIterator++] = x + 1;
				lineIndex[lineIterator++] = horizontalLevels + x + 1;

			}
		}
	}

	for(int y = 1; y < verticalLevels - 1; y++) {
		for(int x = 0; x < horizontalLevels; x++) {
			if(x == 0) {
				// Lewo
				lineIndex[lineIterator++] = y * verticalLevels + x;
				lineIndex[lineIterator++] = (y + 1) * verticalLevels + x;
				// Prawo
				lineIndex[lineIterator++] = y * verticalLevels + x + 1;
				lineIndex[lineIterator++] = (y + 1) * verticalLevels + x + 1;
				// Dó³
				lineIndex[lineIterator++] = (y + 1) * verticalLevels + x ;
				lineIndex[lineIterator++] = (y + 1) * verticalLevels + x + 1;
			}
			else {
				if(x == horizontalLevels - 1) {
					// Dó³
					lineIndex[lineIterator++] = (y + 1) * verticalLevels + x ;
					lineIndex[lineIterator++] = (y + 1) * verticalLevels ;
				}
				else {
					// Prawo
					lineIndex[lineIterator++] = y * verticalLevels + x + 1;
					lineIndex[lineIterator++] = (y + 1) * verticalLevels + x + 1;
					// Dó³
					lineIndex[lineIterator++] = (y + 1) * verticalLevels + x ;
					lineIndex[lineIterator++] = (y + 1) * verticalLevels + x + 1;
				}
			}
		}
	}

	glGenBuffers(1, &balloonLineIndex);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, balloonLineIndex);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(lineIndex), lineIndex, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// Vertex Array Buffer

	glGenVertexArrays(1, &balloonVAO);
	glBindVertexArray(balloonVAO);

	glBindBuffer(GL_ARRAY_BUFFER, balloonBufferObject);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)(numberOfVertices * 4 * 4));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)(numberOfVertices * 4 * 2 * 4));

	glBindVertexArray(0);

	// Konstrukcja Balonu
	basket.construct(lowestRadius, lowestY);
}
void Balloon::render(int renderType) {
	glBindVertexArray(balloonVAO);
	switch(renderType) {
		case 0:
			glDrawArrays(GL_POINTS, 0, numberOfVertices);
			break;
		case 1:
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangleIndexBuffer);
			glDrawElements(GL_TRIANGLES, numbOfTriangles * 3, GL_UNSIGNED_INT, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, revTriangleIndexBuffer);
			glDrawElements(GL_TRIANGLES, numbOfTriangles * 3, GL_UNSIGNED_INT, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, balloonLineIndex);
			glDrawElements(GL_LINES, balloonNumberOfLineIndices, GL_UNSIGNED_SHORT, 0);
			break;
		case 2:
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, balloonLineIndex);
			glDrawElements(GL_LINES, balloonNumberOfLineIndices, GL_UNSIGNED_SHORT, 0);
			break;
	}
	basket.render(renderType);
}