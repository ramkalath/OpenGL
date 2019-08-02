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

// SOIL includes
#include <SOIL/SOIL.h>

// User define includes
#include <iostream>
#include <vector>

#include "../include/shader.h"

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

struct Texture
{
	unsigned int id;
	std::string type;
};

struct Mesh
{
	std::vector<Vertex> vertices;
	std::vector<Texture> diffusemaps;
	std::vector<Texture> specularmaps;
	std::vector<unsigned int> indices;
	unsigned int VAO, VBO, EBO;
};

class Modelloader
{
public:
	Modelloader(const std::string path);
	glm::mat4 modelmatrix;
	void Draw(Shader);

private:
	std::string directory;
	const aiScene *scene;
	std::vector<Mesh> meshes;
	unsigned int num_meshes;
	unsigned int num_indices;

	void ProcessNode(aiNode *);
	void ProcessMeshes(aiNode *);
};
