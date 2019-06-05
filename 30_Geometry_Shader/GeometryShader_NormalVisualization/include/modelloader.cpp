/*****************************************************************************
 * Author : Ram
 * Date : 18/Dec/2018
 * Email : ramkalath@gmail.com
 * Breif Description : this function does model loading
 * Detailed Description : Model loading using assimp's node tree
 *****************************************************************************/
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

// Usual includes
#include <iostream>
#include <vector>

#include "../include/vert_frag_shader.h"
#include "../include/vert_geom_frag_shader.h"

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
	aiString path;
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

// constructor
Modelloader::Modelloader(std::string path)
{
	Assimp::Importer importer;
	this->scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
	if(!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cerr << "\033[0;91m ERROR::ASSIMP:: \033[0m\n" << importer.GetErrorString() << std::endl;
		return;
	}

	// initializations
	this->num_meshes = 0;
	this->num_indices = 0;
	this->directory = path.substr(0, path.find_last_of('/')).c_str();

	this->ProcessNode(scene->mRootNode);
}

// Node processor
void Modelloader::ProcessNode(aiNode *node)
{
	if(node->mNumChildren==0) 
		ProcessMeshes(node);
	else 
		for(unsigned int i=0; i<node->mNumChildren; i++)
			this->ProcessNode(node->mChildren[i]);
}

// Mesh processor
void Modelloader::ProcessMeshes(aiNode *node)
{
	for(unsigned int i=0; i<node->mNumMeshes; i++)
	{
		Mesh m;
		aiMesh *mesh = this->scene->mMeshes[node->mMeshes[i]];
		Vertex vtx;

		// Load Vertex data -------------------------------------------------------------------------
		aiColor4D ambient, diffuse, specular;
		if(this->scene->HasMaterials()) {
			aiMaterial *mtl = scene->mMaterials[mesh->mMaterialIndex];
			aiGetMaterialColor(mtl, AI_MATKEY_COLOR_AMBIENT, &ambient);
			aiGetMaterialColor(mtl, AI_MATKEY_COLOR_DIFFUSE, &diffuse);
			aiGetMaterialColor(mtl, AI_MATKEY_COLOR_SPECULAR, &specular);
		}
		for(unsigned int k=0; k<mesh->mNumVertices; k++) {
			// positions, normals and textures
			vtx.position = glm::vec3(mesh->mVertices[k].x, mesh->mVertices[k].y, mesh->mVertices[k].z);
			vtx.normal = glm::vec3(mesh->mNormals[k].x, mesh->mNormals[k].y, mesh->mNormals[k].z);
			if(mesh->mTextureCoords[0])
				vtx.texcoord = glm::vec2(mesh->mTextureCoords[0][k].x, mesh->mTextureCoords[0][k].y);
			else vtx.texcoord = glm::vec2(0.0f, 0.0f);
			// load colors
			vtx.colorambient = glm::vec3(ambient.r, ambient.g, ambient.b);
			vtx.colordiffuse = glm::vec3(diffuse.r, diffuse.g, diffuse.b);
			vtx.colorspecular = glm::vec3(specular.r, specular.g, specular.b);
			vtx.shininess = 16.0f;

			m.vertices.push_back(vtx);
		}
		// mesh data and size
		float *MeshData = &(m.vertices[0].position.x);
		unsigned int MeshDataSize = sizeof(float)*18*mesh->mNumVertices;

		// Load indices ------------------------------------------------------------------------------
		for(unsigned int l=0; l<mesh->mNumFaces; l++) {
			m.indices.push_back(mesh->mFaces[l].mIndices[0]);
			m.indices.push_back(mesh->mFaces[l].mIndices[1]);
			m.indices.push_back(mesh->mFaces[l].mIndices[2]);
		}

		// indices data and size
		unsigned int *Indices = m.indices.data();
		unsigned int IndicesSize = m.indices.size()*sizeof(unsigned int);

		// setting up textures --------------------------------------------------------------------
		if(mesh->mMaterialIndex>=0)
		{
			aiMaterial *material = this->scene->mMaterials[mesh->mMaterialIndex];

			// Diffuse maps loading
			for(unsigned int i=0; i<material->GetTextureCount(aiTextureType_DIFFUSE); i++) {
				Texture tex;
				tex.type = "diffuse_texture";
				material->GetTexture(aiTextureType_DIFFUSE, i, &tex.path);
				tex.path = this->directory + '/' + tex.path.C_Str();

				glGenTextures(1, &tex.id);
				glBindTexture(GL_TEXTURE_2D, tex.id);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				int texture_width, texture_height;
				unsigned char *image = SOIL_load_image(tex.path.C_Str(), &texture_width, &texture_height, 0, SOIL_LOAD_RGB);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture_width, texture_height, 0, GL_RGB, GL_UNSIGNED_BYTE, image); 
				glGenerateMipmap(GL_TEXTURE_2D);
				SOIL_free_image_data(image);
				glBindTexture(GL_TEXTURE_2D, 0);
				m.diffusemaps.push_back(tex);
			}

			// Specular maps loading
			for(unsigned int i=0; i<material->GetTextureCount(aiTextureType_SPECULAR); i++) {
				Texture tex;
				tex.type = "specular_texture";
				material->GetTexture(aiTextureType_SPECULAR, i, &tex.path);
				tex.path = this->directory + '/' + tex.path.C_Str();

				glGenTextures(1, &tex.id);
				glBindTexture(GL_TEXTURE_2D, tex.id);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				int texture_width, texture_height;
				unsigned char *image = SOIL_load_image(tex.path.C_Str(), &texture_width, &texture_height, 0, SOIL_LOAD_RGB);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture_width, texture_height, 0, GL_RGB, GL_UNSIGNED_BYTE, image); 
				glGenerateMipmap(GL_TEXTURE_2D);
				SOIL_free_image_data(image);
				glBindTexture(GL_TEXTURE_2D, 0);
				m.specularmaps.push_back(tex);
			}
		}

		// Setup Mesh -----------------------------------------------------------------------------
		glGenVertexArrays(1, &(m.VAO));
		glBindVertexArray(m.VAO);

		glGenBuffers(1, &(m.VBO));
		glBindBuffer(GL_ARRAY_BUFFER, m.VBO);
		glBufferData(GL_ARRAY_BUFFER, MeshDataSize, MeshData, GL_STATIC_DRAW);

		glGenBuffers(1, &(m.EBO));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m.EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, IndicesSize, Indices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 18*sizeof(GLfloat), (GLvoid*)0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 18*sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat)));
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 18*sizeof(GLfloat), (GLvoid*)(6*sizeof(GLfloat)));
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 18*sizeof(GLfloat), (GLvoid*)(8*sizeof(GLfloat)));
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 18*sizeof(GLfloat), (GLvoid*)(11*sizeof(GLfloat)));
		glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, 18*sizeof(GLfloat), (GLvoid*)(14*sizeof(GLfloat)));
		glVertexAttribPointer(6, 1, GL_FLOAT, GL_FALSE, 18*sizeof(GLfloat), (GLvoid*)(17*sizeof(GLfloat)));

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);
		glEnableVertexAttribArray(4);
		glEnableVertexAttribArray(5);
		glEnableVertexAttribArray(6);

		glBindVertexArray(0);
		// --------------------------------------------------------------------------------------

		this->meshes.push_back(m);
	}
}

// Draw
void Modelloader::Draw(Shader shader)
{
	for(unsigned int i=0; i<this->meshes.size(); i++)
	{
		glBindVertexArray(meshes[i].VAO);
		unsigned int k=0;
		for(unsigned int j=0; j<meshes[i].diffusemaps.size(); j++) {
			glUniform1i(glGetUniformLocation(shader.program, "diffuse_texture"), j);
			glActiveTexture(GL_TEXTURE0+j);
			glBindTexture(GL_TEXTURE_2D, meshes[i].diffusemaps[j].id);
			k++;
		}
		for(unsigned int j=0; j<meshes[i].specularmaps.size(); j++) {
			glUniform1i(glGetUniformLocation(shader.program, "specular_texture"), j+k);
			glActiveTexture(GL_TEXTURE0+j+k);
			glBindTexture(GL_TEXTURE_2D, meshes[i].specularmaps[j].id);
		}
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawElements(GL_TRIANGLES, meshes[i].indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
}
