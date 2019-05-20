#pragma once
#define GLEW_STATIC
#include <GL/glew.h>

// glm includes
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Assimp Includes
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

// User define includes
#include <iostream>
#include <vector>

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texcoord;

	glm::vec3 colorambient;
	glm::vec3 colordiffuse;
	glm::vec3 colorspecular;
	float shininess;
};

class modelloader
{
public:
	const aiScene *scene;
	glm::mat4 modelmatrix{1.0f};

	modelloader(const std::string path);
	void PrintNodeStack();
	void RenderModel();

private:
	float *MeshData;
	unsigned int MeshDataSize;
	unsigned int *Indices;
	unsigned int IndicesSize;
	unsigned int VBO, VAO, EBO;
	std::vector<Vertex> meshdata;
	std::vector<unsigned int>indices;
	std::vector<aiNode*> nodestack;

	void NodeTreeTraversal(aiNode*);
	void ProcessMeshes();
	void CreateOpenGLDataStructures();
};
