#include "Functions.h"
#include "Triangle.h"

float dotProduct(const glm::vec3& v1, const glm::vec3& v2) {
  return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

AdjacencyList createGridAdjacencyList(vector<glm::vec4>& vertices, glm::uvec2& dimensions, bool loop) {

  static unsigned int tempTime = glutGet(GLUT_ELAPSED_TIME);

  AdjacencyList adjacencyList;
  adjacencyList.resize(vertices.size());

  int trianglesPerLevel = (!loop ? (dimensions.x - 1) * 2 : dimensions.x * 2);
  cout << "Creating Adjacency List For: " << vertices.size() << " Vertices" << endl;

  int index = 0;
  for(int y = 0; y < (int)dimensions.x; y++) {
    for(int x = 0; x < (int)dimensions.y; x++) {
      index = x + y * dimensions.y;


      // Lewa Kolumna
      if(x == 0) {
	// Je¿eli nie jest ostatni wiersz
	if(y != dimensions.y - 1) {
	  // Dodajemy Z Poprzedniego Poziomu Trףjk¹t
	  adjacencyList[index].push_back(trianglesPerLevel * y);
	  adjacencyList[index].push_back(trianglesPerLevel * y + 1);
	}
	// Je¿eli jest nie pierwszy albo ostatni
	if(y != 0 || y == dimensions.y - 1) {
	  adjacencyList[index].push_back(trianglesPerLevel * (y - 1) + 1);
	}
      }
      // Prawa Kolumna
      else if(x == dimensions.x - 1) {

	/*if(y != dimensions.y - 1){
	  adjacencyList[index].push_back(trianglesPerLevel - 2 + trianglesPerLevel * y);
	  adjacencyList[index].push_back(trianglesPerLevel - 2 + trianglesPerLevel * y + 1) ;

	  }
	  if(y != 0) {
	  adjacencyList[index].push_back(trianglesPerLevel - 2 + trianglesPerLevel * (y - 1));
	  adjacencyList[index].push_back(trianglesPerLevel - 2 + trianglesPerLevel * (y - 1) + 1) ;
	  }*/

	// Je¿eli nie jest ostatni wiersz
	if(y != dimensions.y - 1) {
	  // Dodajemy Z Poprzedniego Poziomu Trףjk¹t
	  adjacencyList[index].push_back(trianglesPerLevel * y + trianglesPerLevel - 2);
	  adjacencyList[index].push_back(trianglesPerLevel * y + 1 + trianglesPerLevel - 2);
	}
	// Je¿eli jest nie pierwszy albo ostatni
	if(y != 0 || y == dimensions.y - 1) {
	  adjacencyList[index].push_back(trianglesPerLevel * (y - 1) + 1 + trianglesPerLevel - 2);
	}

      } // Wewnךtrzne Kolumny
      else {
	// Pierwszy Rz¹d
	if(y == 0) {
	  adjacencyList[index].push_back((x - 1) * 2);
	  adjacencyList[index].push_back((x - 1) * 2 + 1);
	  adjacencyList[index].push_back((x - 1) * 2 + 2);
	}// Ostatni Rz¹d
	else if(y == dimensions.y - 1) {
	  adjacencyList[index].push_back((y - 1) *(dimensions.x - 1) * 2 + x * 2);
	  adjacencyList[index].push_back((y - 1) *(dimensions.x - 1) * 2 + 1 + x * 2);
	}// �rodek
	else {
	  // Gףra
	  adjacencyList[index].push_back((x - 1) * 2 + 1 + (y - 1) * trianglesPerLevel);
	  adjacencyList[index].push_back((x - 1) * 2 + 1 + (y - 1) * trianglesPerLevel + 1);
	  adjacencyList[index].push_back((x - 1) * 2 + 1 + (y - 1) * trianglesPerLevel + 2);
	  // Dף³
	  adjacencyList[index].push_back(y * trianglesPerLevel + (x * 2) - 2);
	  adjacencyList[index].push_back(y * trianglesPerLevel + (x * 2) - 1);
	  adjacencyList[index].push_back(y * trianglesPerLevel + (x * 2));
	}
      }
      if(!((x + y * dimensions.y) % 1000))cout << "  Done: " << x + y * dimensions.y << " [ " << int((x + y * dimensions.y) / float(dimensions.x * dimensions.y) * 10000.0f) / 100 << " %] " << endl;
    }
  }
  cout << "  Done: " << dimensions.x * dimensions.y << endl;
  cout << "Finished \n\n";

  cout << "Adjacency List Took: " << glutGet(GLUT_ELAPSED_TIME) - tempTime << endl;
  return adjacencyList;
}
vector<glm::uvec2> createGridLineIndex(int width, int height) {
  const unsigned long int numbOfLines = 4 + 3 * (width - 2) + (3 + 2 * (width - 2)) * (height - 1);
  vector<glm::uvec2> indexVector;
  indexVector.resize(numbOfLines);

  int it = 0;
  for(int y = 0; y < height - 1; y++) {
    for(int x = 0; x < width - 1; x++) {
      // Gףra
      if(y == 0) indexVector[it++] = glm::uvec2(y * height + x, y * height + 1 + x);
      // Lewo
      if(x == 0) indexVector[it++] = glm::uvec2(y * height, (y + 1) * height);
      //Prawo
      indexVector[it++] = glm::uvec2(y * height + x + 1, (y + 1) * height + x + 1);
      // Dף³
      indexVector[it++] = glm::uvec2((y + 1) * height + x, (y + 1) * height + 1 + x);
    }
  }
  return indexVector;
}
vector<glm::uvec3> createGridTriangleIndex(int width, int height) {
  vector<glm::uvec3> triangles;
  triangles.resize((width - 1) * (height - 1) * 2);

  int it = 0;
  for(int y = 0; y < width - 1; y++) {
    for(int x = 0; x < height - 1; x++) {
      triangles[it].x = x + y * width;
      triangles[it].y = (x + 1) % width + y * width;
      triangles[it].z = (x + 1) % width + (y + 1) * width;

      it++;

      triangles[it].x = x + y * width;
      triangles[it].y = (x + 1) % width + (y + 1) * width;
      triangles[it].z = x + (y + 1) * width;

      it++;
    }
  }
  return triangles;
}
vector<glm::vec3> getNormals(vector<glm::vec4>& vertices, vector<glm::uvec3>& faceIndex, AdjacencyList& adjacencyList) {
  vector<glm::vec3> normals;
  normals.resize(vertices.size());

  static unsigned int tempTime = glutGet(GLUT_ELAPSED_TIME);

  vector<glm::vec3> faceNormals = calculateFaceNormals(vertices, faceIndex);

  cout << "FaceNormals Took: " << glutGet(GLUT_ELAPSED_TIME) - tempTime << endl;
  glm::vec3 combinedNormal;
  tempTime = glutGet(GLUT_ELAPSED_TIME);
  cout << "Calculating Vertex Normals For: " << vertices.size() << " Vertices" << endl;

  int it = 0;
  for(auto i = vertices.begin(); i != vertices.end(); i++) {
    // Getting triangles that contain given vertex
    if(adjacencyList.size() == 0) {
      list<int>& indexes = getTriangleIndexes(it, faceIndex);

      for(auto j = indexes.begin(); j != indexes.end(); j++)normals[it] += faceNormals[*j];
      normals[it] = glm::normalize(normals[it]);
      it++;
    } else {
      list<int>& indexes = adjacencyList[it];

      for(auto j = indexes.begin(); j != indexes.end(); j++)normals[it] += faceNormals[*j];
      normals[it] = glm::normalize(normals[it]);
      it++;
    }
    //indexes = getTriangleIndexes(it, faceIndex);


    //// Summing Up All The Normals Of Faces Connected To Vertex
    //for(auto j = indexes.begin(); j != indexes.end(); j++)normals[it] += faceNormals[*j];
    //normals[it] = glm::normalize(normals[it]);
    //it++;

    if(!(it % 5000))cout << "  Done: " << it << " [" << int((it / (float)vertices.size()) * 10000.0f) / 100 << " %]" << endl;
  }
  cout << "  Done: " << vertices.size() << endl;
  cout << "Finished \n\n";

  //cout << "The Rest Took: " << glutGet(GLUT_ELAPSED_TIME) - tempTime << endl;

  return normals;
}
vector<glm::vec3> calculateFaceNormals(vector<glm::vec4>& vertices, vector<glm::uvec3>& faceIndex) {
  cout << "Calculating Face Normals For: " << faceIndex.size() << " Triangles \n";
  vector<glm::vec3> faceNormals;
  faceNormals.resize(faceIndex.size());

  int it = 0;

  for(auto i = faceIndex.begin(); i != faceIndex.end(); i++) {
    faceNormals[it++] = glm::cross(glm::vec3(vertices[i->x] - vertices[i->y]), glm::vec3(vertices[i->z] - vertices[i->y]));
    if(!(distance(faceIndex.begin(), i) % 10000)) cout << "  Done: " << distance(faceIndex.begin(), i) << " [" << (int(distance(faceIndex.begin(), i) / (float)faceIndex.size() * 10000.0f)) / 100 << " %]" << endl;
  }

  cout << "  Done: " << faceIndex.size() << endl;
  cout << "Finished \n\n";
  return faceNormals;
}

list<int> getTriangleIndexes(int srcIndex, vector<glm::uvec3>& faceIndex) {
  list<int> triIndex;
  for(auto i = faceIndex.begin(); i != faceIndex.end(); i++) {
    int index = distance(faceIndex.begin(), i);
    if(i->x == srcIndex || i->y == srcIndex || i->z == srcIndex) triIndex.push_back(index);
  }
  return triIndex;
}
void reverseTriangleIndexes(vector<glm::uvec3>& triangles) {
  unsigned int temp;
  for(auto i = triangles.begin(); i != triangles.end(); i++) {
    temp = i->x;
    i->x = i->z;
    i->z = temp;
  }
}
void addResersedTriangleIndexes(vector<glm::uvec3>& triangles) {
  int orgSize = triangles.size() * 2;
  triangles.resize(orgSize * 2);
  for(int i = 0; i < orgSize ; i++) {
    glm::uvec3& currVec = triangles[i];
    triangles[i + orgSize] = glm::uvec3(currVec.z, currVec.y, currVec.x);
  }
}
void addIndexedQuad(vector<glm::uvec3>& triangles, glm::uvec4 indexes) {
  triangles.push_back(glm::uvec3(indexes.x, indexes.y, indexes.z));
  triangles.push_back(glm::uvec3(indexes.x, indexes.z, indexes.w));
}

void translateVec4(vector<glm::vec4>& vertices, glm::vec4 delta) {
  for(auto i = vertices.begin(); i != vertices.end(); i++) {
    (*i) += delta;
  }
}
float getMaxVec4(vector<glm::vec4>& vertices, int dimension) {
  float max = 0;
  for(auto i = vertices.begin(); i != vertices.end(); i++) {
    float temp = selectValVec4(*i, dimension);
    if(i == vertices.begin()) max = temp;
    else if(temp > max)  max = temp;
  }
  return max;
}
float getMinVec4(vector<glm::vec4>& vertices, int dimension) {
  float min = 0;
  for(auto i = vertices.begin(); i != vertices.end(); i++) {
    float temp = selectValVec4(*i, dimension);
    if(i == vertices.begin()) min = temp;
    else if(temp < min)  min = temp;
  }
  return min;
}
float selectValVec4(glm::vec4& vector, int dimension) {
  switch(dimension){
  case 1: return vector.x;
  case 2: return vector.y;
  case 3: return vector.z;
  case 4: return vector.w;

  default:
    cout << "YOLO\n";
  }
  return 0;
}

GLuint loadBMP_custom(const char * imagePath) {
  unsigned char header[54];
  unsigned int dataPos;
  unsigned int width, height;
  unsigned int imageSize;

  unsigned char * data;

  FILE * file = fopen(imagePath, "rb");
  if(!file) {
    cout << "File doesn't exist !!!\n";
    return 0;
  }

  if(fread(header, 1, 54, file) != 54){ // If not 54 bytes read : problem
    printf("Not a correct BMP file\n");
    return false;
  }
  if(header[0] != 'B' || header[1] != 'M'){
    printf("Not a correct BMP file\n");
    return 0;
  }
  // Read ints from the byte array
  dataPos = *(int*)&(header[0x0A]);
  imageSize = *(int*)&(header[0x22]);
  width = *(int*)&(header[0x12]);
  height = *(int*)&(header[0x16]);

  // Some BMP files are misformatted, guess missing information
  if(imageSize == 0)    imageSize = width*height * 3; // 3 : one byte for each Red, Green and Blue component
  if(dataPos == 0)      dataPos = 54; // The BMP header is done that way

  // Create a buffer
  data = new unsigned char[imageSize];

  // Read the actual data from the file into the buffer
  fread(data, 1, imageSize, file);

  //Everything is in memory now, the file can be closed
  fclose(file);

  // Create one OpenGL texture
  GLuint textureID;
  glGenTextures(1, &textureID);

  // "Bind" the newly created texture : all future texture functions will modify this texture
  glBindTexture(GL_TEXTURE_2D, textureID);

  // Give the image to OpenGL
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

  return textureID;
}

GLuint CreateShader(GLenum eShaderType, const string & strShaderFile) {
  GLuint shader = glCreateShader(eShaderType);
  string shaderText(std::istreambuf_iterator<char>(std::ifstream(strShaderFile).rdbuf()), std::istreambuf_iterator<char>());

  const char * strFileData = shaderText.c_str();
  glShaderSource(shader, 1, &strFileData, NULL);

  glCompileShader(shader);

  //Error Stuff

  GLint status;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
  if(status == GL_FALSE)
  {
    GLint infoLogLength;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);

    GLchar *strInfoLog = new GLchar[infoLogLength + 1];
    glGetShaderInfoLog(shader, infoLogLength, NULL, strInfoLog);

    const char *strShaderType = NULL;
    switch(eShaderType)
    {
    case GL_VERTEX_SHADER: strShaderType = "vertex"; break;
    case GL_GEOMETRY_SHADER: strShaderType = "geometry"; break;
    case GL_FRAGMENT_SHADER: strShaderType = "fragment"; break;
    }

    fprintf(stderr, "Compile failure in %s shader:\n%s\n", strShaderType, strInfoLog);
    delete[] strInfoLog;
  }

  return shader;
}
GLuint CreateProgram(const vector<GLuint>& shaderList) {
  GLuint program = glCreateProgram();
  for(size_t iLoop = 0; iLoop < shaderList.size(); iLoop++) glAttachShader(program, shaderList[iLoop]);
  glLinkProgram(program);

  // Error Stuff
  GLint status;
  glGetProgramiv(program, GL_LINK_STATUS, &status);
  if(status == GL_FALSE)
  {
    GLint infoLogLength;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);

    GLchar *strInfoLog = new GLchar[infoLogLength + 1];
    glGetProgramInfoLog(program, infoLogLength, NULL, strInfoLog);
    fprintf(stderr, "Linker failure: %s\n", strInfoLog);
    delete[] strInfoLog;
  }

  for(size_t iLoop = 0; iLoop < shaderList.size(); iLoop++)
    glDetachShader(program, shaderList[iLoop]);

  return program;
}
