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
	glm::mat4 modelmatrix;

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

// constructor
modelloader::modelloader(const std::string path)
{
	Assimp::Importer importer;
	scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
	if(!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cerr << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
		return;
	}

	NodeTreeTraversal(scene->mRootNode);
	ProcessMeshes();
	CreateOpenGLDataStructures();
	RenderModel();
}

// Iterates through the list of nodes in a node tree
void modelloader::NodeTreeTraversal(aiNode *node)
{
	if(node->mNumChildren==0)
		nodestack.push_back(node);
	else
		for(unsigned int i=0; i<node->mNumChildren; i++)
			this->NodeTreeTraversal(node->mChildren[i]);
}

// dummy print function after storing all the nodes into the nodestack stl::vector
void modelloader::PrintNodeStack()
{
	std::cout << this->nodestack.size() << std::endl;
}

// processes all the meshes of the nodes by stacking them linearly.
void modelloader::ProcessMeshes()
{
	// currently this method loads vertex positions, normals, textures; 
	// also loads material info such as ambient, diffuse and specular colors with shininess as 16.0f
	// check wether object has normals, materials and texture infos
	Vertex vertex;
	unsigned int offset_faces=0;

	for(unsigned int i=0; i<this->nodestack.size(); i++)
	{
		aiNode *node = nodestack[i];
		for(unsigned int j=0; j<node->mNumMeshes; j++)
		{
			aiMesh *mesh = this->scene->mMeshes[node->mMeshes[j]];

			aiColor4D ambient;
			aiColor4D diffuse;
			aiColor4D specular;

			if(this->scene->HasMaterials()) {
				aiMaterial *mtl = scene->mMaterials[mesh->mMaterialIndex];
				aiGetMaterialColor(mtl, AI_MATKEY_COLOR_AMBIENT, &ambient);
				aiGetMaterialColor(mtl, AI_MATKEY_COLOR_DIFFUSE, &diffuse);
				aiGetMaterialColor(mtl, AI_MATKEY_COLOR_SPECULAR, &specular);
			}

			for(unsigned int k=0; k<mesh->mNumVertices; k++)
			{
				// positions and normals
				vertex.position = glm::vec3(mesh->mVertices[k].x, mesh->mVertices[k].y, mesh->mVertices[k].z); // load positions
				vertex.normal = glm::vec3(mesh->mNormals[k].x, mesh->mNormals[k].y, mesh->mNormals[k].z); // load normals

				// load textures
				if(this->scene->HasTextures())
					vertex.texcoord = glm::vec2(mesh->mTextureCoords[0][k].x, mesh->mTextureCoords[0][k].y);
				else vertex.texcoord = glm::vec2(0.0f, 0.0f);

				// load materials
				vertex.colorambient = glm::vec3(ambient.r, ambient.g, ambient.b);
				vertex.colordiffuse = glm::vec3(diffuse.r, diffuse.g, diffuse.b);
				vertex.colorspecular = glm::vec3(specular.r, specular.g, specular.b);
				vertex.shininess = 16.0f;

				// push back all the data for each vertex
				meshdata.push_back(vertex);
			}

			for(unsigned int l=0; l<mesh->mNumFaces; l++) {
				this->indices.push_back(mesh->mFaces[l].mIndices[0]+offset_faces);
				this->indices.push_back(mesh->mFaces[l].mIndices[1]+offset_faces);
				this->indices.push_back(mesh->mFaces[l].mIndices[2]+offset_faces);
			}
			offset_faces = offset_faces+mesh->mNumVertices;
		}
	}
	this->MeshData = &meshdata[0].position.x;
	this->MeshDataSize = meshdata.size() * 18 * sizeof(float);

	this->Indices = indices.data();
	this->IndicesSize = indices.size()*sizeof(unsigned int);
}

void modelloader::CreateOpenGLDataStructures()
{
	glGenVertexArrays(1, &(this->VAO));
	glBindVertexArray(this->VAO);

	glGenBuffers(1, &(this->VBO));
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, this->MeshDataSize, this->MeshData, GL_STATIC_DRAW);

	glGenBuffers(1, &(this->EBO));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->IndicesSize, this->Indices, GL_STATIC_DRAW);

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
}

void modelloader::RenderModel() 
{
	glBindVertexArray(this->VAO);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawElements(GL_TRIANGLES, this->IndicesSize/sizeof(unsigned int), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
