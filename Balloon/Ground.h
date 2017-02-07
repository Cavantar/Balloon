#pragma once 

#include "Includes.h"

class ColorSet{
public:
	int bottomValue;
	glm::vec4 color;

	ColorSet(int bottomValue, glm::vec4& color): bottomValue(bottomValue), color(color) {  };
};

class Ground{
public:
	friend class GrowTerrainCommand;

	void construct();
	void reConstruct();
	void render(int renderType);
	bool isColliding(glm::vec3& position);
	bool isColliding(glm::vec3& position, float radius);

	float getHighestPoint(glm::vec3& position);
	vector<glm::vec4>& getVertices(){ return verticesVec; }

	float maxY;
	float minY;

	float gridLength;
	int desiredSideLength = 6;
	int sideLength = 6;

private:

	int groundWidth;
	int groundHeight;

	vector<float> rawData;	     // Data for gfx
	vector<glm::vec4> verticesVec; // For Collision and Stuff
	vector<glm::vec4> colorsVec;  // Color For Vertices

	vector<ColorSet> colorPallette;

	AdjacencyList adjacencyList; 
	vector<glm::uvec3> triangles; 

	GLuint groundBufferObject;
	GLuint groundLineIndex;
	GLuint triangleIndexBuffer;

	GLuint groundVAO;

	int groundArraySize;
	int numbOfVertices;
	int numbOfLines;
	int numbOfTriangles;

	void createColorSet();
	glm::vec4& getColor(int perc);

	void setVerticesColors(vector<glm::vec4>& colorsVec, vector<glm::vec4>& vertices);
	vector<glm::vec4*> getVerticesAtPoint(glm::vec3& position);
};


