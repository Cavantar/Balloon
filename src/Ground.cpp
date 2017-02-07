#include "Ground.h"
#include "TerrainGenerator.h"

void Ground::construct() {
	groundWidth = (int)pow(2, sideLength) + 1;
	groundHeight = groundWidth; 

	numbOfVertices = groundWidth * groundHeight;
	groundArraySize = numbOfVertices * 4 * 2 + numbOfVertices * 3;

	rawData.resize(groundArraySize);
	gridLength = 15.0f;
	
	// Generowanie Terenu
	// Polecana ostroœæ to dla 
	// 129 - 40 - grid = 1.0f
	// 257 - 80 - 120

	TerrainGenerator::sharpness = 200;
	verticesVec = TerrainGenerator::generateTerrain(sideLength, glm::vec4(0.5f, 0.5f, 0.5f, 0.5f), gridLength);

	/*for(int i = 0; i < groundHeight; i++) {
		for(int j = 0; j < groundWidth; j++) {
			glm::vec4 tempVec;

			tempVec.x = j * gridLength - groundWidth / 2.0f * gridLength;
			tempVec.y =  sin(3.14f * i / (5));
			tempVec.z = i * gridLength - groundHeight / 2.0f * gridLength ;
			tempVec.w = 1.0f;

			verticesVec.push_back(tempVec);
		}
	}*/


	memcpy(&rawData[0], &verticesVec[0], verticesVec.size() * sizeof(glm::vec4));

	// Kolorowanie !!!

	createColorSet();
	colorsVec.resize(verticesVec.size());

	setVerticesColors(colorsVec, verticesVec);
	memcpy(&rawData[groundWidth * groundHeight * 4], &colorsVec[0], colorsVec.size() * sizeof(glm::vec4));

	// Indeksy Linii
	vector<glm::uvec2> lineIndexVec = createGridLineIndex(groundWidth, groundHeight);
	numbOfLines = lineIndexVec.size();

	glGenBuffers(1, &groundLineIndex);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, groundLineIndex);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, lineIndexVec.size() * sizeof(glm::uvec2), &lineIndexVec[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// Indeksy Trójk¹tów
	cout << "Indexing Triangles \n";
	triangles = createGridTriangleIndex(groundWidth, groundHeight);
	numbOfTriangles = triangles.size();

	glGenBuffers(1, &triangleIndexBuffer);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangleIndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, triangles.size() * sizeof(glm::uvec3), &triangles[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	cout << endl;

	// Lista s¹siedztwa
	adjacencyList = createGridAdjacencyList(verticesVec, glm::uvec2(groundWidth, groundHeight));

	//Normalne 
	vector<glm::vec3>& normals = getNormals(verticesVec, triangles, adjacencyList);
	memcpy(&rawData[groundWidth * groundHeight * 4 * 2], &normals[0], normals.size() * sizeof(glm::vec3));

	// Kopiowanie do grafiki
	glGenBuffers(1, &groundBufferObject);

	glBindBuffer(GL_ARRAY_BUFFER, groundBufferObject);
	glBufferData(GL_ARRAY_BUFFER, groundArraySize * 4, &rawData[0], GL_STREAM_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Tworzenie VAO

	glGenVertexArrays(1, &groundVAO);
	glBindVertexArray(groundVAO);

	glBindBuffer(GL_ARRAY_BUFFER, groundBufferObject);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *)(numbOfVertices * 4 * 4));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid *)(numbOfVertices * 4 * 2 * 4));

	glBindVertexArray(0);
}
void Ground::reConstruct() {

	// Wierzcho³ki 
	verticesVec = TerrainGenerator::generateTerrain(sideLength, glm::vec4(0.5f, 0.5f, 0.5f, 0.5f), gridLength);
	memcpy(&rawData[0], &verticesVec[0], verticesVec.size() * sizeof(glm::vec4));
	
	// Colory
	setVerticesColors(colorsVec, verticesVec);
	memcpy(&rawData[groundWidth * groundHeight * 4], &colorsVec[0], colorsVec.size() * sizeof(glm::vec4));

	//Normalne 
	vector<glm::vec3>& normals = getNormals(verticesVec, triangles, adjacencyList);
	memcpy(&rawData[groundWidth * groundHeight * 4 * 2], &normals[0], normals.size() * sizeof(glm::vec3));

	// Kopiowanie do grafiki

	glBindBuffer(GL_ARRAY_BUFFER, groundBufferObject);
	glBufferSubData(GL_ARRAY_BUFFER, 0, groundArraySize * 4 ,& rawData[0]);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

}
void Ground::render(int renderType) {
	
	glBindVertexArray(groundVAO);
	
	switch(renderType) {
		case 0:
			glDrawArrays(GL_POINTS, 0, numbOfVertices);
			break;
		case 1:
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, groundLineIndex);
			glDrawElements(GL_LINES, numbOfLines * 2, GL_UNSIGNED_INT, 0);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangleIndexBuffer);
			glDrawElements(GL_TRIANGLES, numbOfTriangles * 3, GL_UNSIGNED_INT, 0);
			break;
		case 2:
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, groundLineIndex);
			glDrawElements(GL_LINES, numbOfLines * 2, GL_UNSIGNED_INT, 0);
			break;
	}
	glBindVertexArray(0);
}
bool Ground::isColliding(glm::vec3& position){
	vector<glm::vec4*> colliding = getVerticesAtPoint(position);
	for(auto i = colliding.begin(); i != colliding.end(); i++) {
		if((*i) != NULL && (*i)->y > position.y) return true;
	}
	return false;
}
bool Ground::isColliding(glm::vec3& position, float radius) {
	static vector<glm::vec3> collVertices(4);

	// One
	collVertices[0] = position;
	collVertices[0].y -= radius;

	collVertices[0].x -= radius;
	collVertices[0].z -= radius;


	// Two
	collVertices[1] = position;
	collVertices[1].y -= radius;

	collVertices[1].x += radius;
	collVertices[1].z -= radius;

	// Three
	collVertices[2] = position;
	collVertices[2].y -= radius;

	collVertices[2].x -= radius;
	collVertices[2].z += radius;

	// Four
	collVertices[3] = position;
	collVertices[3].y -= radius;

	collVertices[3].x += radius;
	collVertices[3].z += radius;


	for(auto i = collVertices.begin(); i != collVertices.end(); i++) {
		if(isColliding(*i))  return true;
	}
	return false;
}
vector<glm::vec4*> Ground::getVerticesAtPoint(glm::vec3& position) {
	vector<glm::vec4*> collidingVert;
	collidingVert.resize(4);
	
	glm::vec3 tempPosition = position;
	float offsetXZ = gridLength * (float)groundWidth * 0.5f;

	tempPosition.x += offsetXZ;
	tempPosition.z += offsetXZ;
	
	tempPosition.x /= gridLength;
	tempPosition.z /= gridLength;

	int index = (int)tempPosition.z * groundWidth + (int)tempPosition.x;
	if(index > 0 && index < (int)verticesVec.size()) collidingVert[0] = &verticesVec[index];
	else collidingVert[0] = NULL;

	index = ((int)tempPosition.z + 1) * groundWidth + (int)tempPosition.x;
	if(index > 0 && index < (int)verticesVec.size()) collidingVert[1] = &verticesVec[index];
	else collidingVert[1] = NULL;
	
	index = ((int)tempPosition.z + 1) * groundWidth + (int)tempPosition.x + 1;
	if(index > 0 && index < (int)verticesVec.size()) collidingVert[2] = &verticesVec[index];
	else collidingVert[2] = NULL;

	index = (int)tempPosition.z * groundWidth + (int)tempPosition.x + 1;
	if(index > 0 && index < (int)verticesVec.size()) collidingVert[3] = &verticesVec[index];
	else collidingVert[3] = NULL;

	return collidingVert;
}
float Ground::getHighestPoint(glm::vec3& position) {
	float highestY = minY;
	vector<glm::vec4*> colliding = getVerticesAtPoint(position);
	for(auto i = colliding.begin(); i != colliding.end(); i++) {
		if((*i) != NULL) {
			if((*i)->y > highestY) highestY = (*i)->y;
		}
	}
	return highestY;
}
void Ground::createColorSet() {
	//80, 63, 60, 18, 15

	colorPallette.push_back(ColorSet(95, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));									// Bia³y
	colorPallette.push_back(ColorSet(80, glm::vec4(200.0f / 255.0f, 200.0f / 255.0f, 200.0f / 255.0f, 1.0f))); // Prawie Bia³y
	colorPallette.push_back(ColorSet(63, glm::vec4(100.0f / 255.0f, 100.0f / 255.0f, 100.0f / 255.0f, 1.0f))); // Szary
	colorPallette.push_back(ColorSet(60, glm::vec4(48.0f / 255.0f, 145.0f / 255.0f, 23.0f / 255.0f, 1.0f)));   // Ciemny Zielony
	colorPallette.push_back(ColorSet(18, glm::vec4(0, 0.6f, 0, 1.0f)));   // Zielony
	colorPallette.push_back(ColorSet(15, glm::vec4(12.0f / 255.0f, 65.0f / 255.0f, 117.0f / 255.0f, 1.0f))); // Niebieski Ciemny
	colorPallette.push_back(ColorSet(0, glm::vec4(23.0f / 255.0f, 113.0f / 255.0f, 201.0f / 255.0f, 1.0f))); // Niebieski Wodny
}
glm::vec4& Ground::getColor(int perc) {
	static glm::vec4 badColor = glm::vec4(1.0f, 0, 0, 1.0f);
	//static int randMax = 1;
	for(auto i = colorPallette.begin(); i != colorPallette.end(); i++) {
		if(perc  >= i->bottomValue) return i->color;
		//if(perc + rand()%randMax - (randMax* 2) >= i->bottomValue) return i->color;
	}
	return badColor;
}
void Ground::setVerticesColors(vector<glm::vec4>& colorsVec, vector<glm::vec4>& vertices) {

	maxY = getMaxVec4(vertices, 2);
	minY = getMinVec4(vertices, 2);

	float deltaY = maxY - minY;

	for(int i = 0; i < numbOfVertices; i++) {
		// Obliczanie delty
		float tempValue = vertices[i].y - minY;
		// Wysokoœæ w procentach
		int percHeight = int((tempValue / deltaY)* 100.0f);

		colorsVec[i] = getColor(percHeight);
	}
}