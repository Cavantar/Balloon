#pragma once 

#include "Includes.h"

float dotProduct(const glm::vec3& v1, const glm::vec3& v2);

// TODO FIX ADJACENCY LIST FOR CYLINDER
AdjacencyList createGridAdjacencyList(vector<glm::vec4>& vertices, glm::uvec2& dimensions, bool loop = false);
vector<glm::uvec2> createGridLineIndex(int width, int height);
vector<glm::uvec3> createGridTriangleIndex(int width, int height);

vector<glm::vec3> getNormals(vector<glm::vec4>& vertices, vector<glm::uvec3>& faceIndex, AdjacencyList& adjacencyList = AdjacencyList());
vector<glm::vec3> calculateFaceNormals(vector<glm::vec4>& vertices, vector<glm::uvec3>& faceIndex);

list<int> getTriangleIndexes(int srcIndex, vector<glm::uvec3>& faceIndex);
void reverseTriangleIndexes(vector<glm::uvec3>& triangles);
void addResersedTriangleIndexes(vector<glm::uvec3>& triangles);
void addIndexedQuad(vector<glm::uvec3>& triangles, glm::uvec4 indexes);

void translateVec4(vector<glm::vec4>& vertices, glm::vec4 delta);
float getMaxVec4(vector<glm::vec4>& vertices, int dimension);
float getMinVec4(vector<glm::vec4>& vertices, int dimension);
float selectValVec4(glm::vec4& vector, int dimension);

GLuint loadBMP_custom(const char * imagepath);

GLuint CreateShader(GLenum eShaderType, const string & strShaderFile);
GLuint CreateProgram(const vector<GLuint>& shaderList);