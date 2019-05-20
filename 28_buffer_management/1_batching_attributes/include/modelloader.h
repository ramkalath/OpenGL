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

class modelloader
{
public:
	const aiScene *scene;
	glm::mat4 modelmatrix;

	modelloader(const std::string path);
	void PrintNodeStack();
	void RenderModel();

private:
	unsigned int *Indices;
	unsigned int IndicesSize;
	unsigned int VBO, VAO, EBO;

	std::vector<glm::vec3> positions;
	std::vector<glm::vec2> texture_coords;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec3> colorambient;
	std::vector<glm::vec3> colordiffuse;
	std::vector<glm::vec3> colorspecular;
	std::vector<float> colorshininess;

	std::vector<unsigned int>indices;
	std::vector<aiNode*> nodestack;

	void NodeTreeTraversal(aiNode*);
	void ProcessMeshes();
	void CreateOpenGLDataStructures();
};
