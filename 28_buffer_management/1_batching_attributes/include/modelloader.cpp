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
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> texture_coords;
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
				positions.push_back(glm::vec3(mesh->mVertices[k].x, mesh->mVertices[k].y, mesh->mVertices[k].z));
				normals.push_back(glm::vec3(mesh->mNormals[k].x, mesh->mNormals[k].y, mesh->mNormals[k].z));
				if(this->scene->HasTextures())
					texture_coords.push_back(glm::vec2(mesh->mTextureCoords[0][k].x, mesh->mTextureCoords[0][k].y));
				else texture_coords.push_back(glm::vec2(0.0f, 0.0f));

				// load materials
				colorambient.push_back(glm::vec3(ambient.r, ambient.g, ambient.b));
				colordiffuse.push_back(glm::vec3(diffuse.r, diffuse.g, diffuse.b));
				colorspecular.push_back(glm::vec3(specular.r, specular.g, specular.b));
				colorshininess.push_back(16.0f);
			}

			for(unsigned int l=0; l<mesh->mNumFaces; l++) {
				this->indices.push_back(mesh->mFaces[l].mIndices[0]+offset_faces);
				this->indices.push_back(mesh->mFaces[l].mIndices[1]+offset_faces);
				this->indices.push_back(mesh->mFaces[l].mIndices[2]+offset_faces);
			}
			offset_faces = offset_faces+mesh->mNumVertices;
		}
	}

	this->Indices = indices.data();
	this->IndicesSize = indices.size()*sizeof(unsigned int);
}

void modelloader::CreateOpenGLDataStructures()
{
	glGenVertexArrays(1, &(this->VAO));
	glBindVertexArray(this->VAO);

	glGenBuffers(1, &(this->VBO));
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, 18*positions.size()*sizeof(float), NULL, GL_STATIC_DRAW);
	// positions
	glBufferSubData(GL_ARRAY_BUFFER, 0, 3*sizeof(float)*positions.size(), positions.data());

	// normals: manual memcpy method for copying data and providing data copy offset
	void *ptr = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	ptr=(char*)ptr+3*sizeof(float)*positions.size(); // without the typecast to char pointer it will give a warning.
	memcpy(ptr, normals.data(), 3*sizeof(float)*normals.size());
	glUnmapBuffer(GL_ARRAY_BUFFER);
	//glBufferSubData(GL_ARRAY_BUFFER, 3*sizeof(float)*positions.size(), 3*sizeof(float)*normals.size(), normals.data());
	
	//texcoords
	glBufferSubData(GL_ARRAY_BUFFER, 3*sizeof(float)*positions.size()+3*sizeof(float)*normals.size(), 2*sizeof(float)*texture_coords.size(), texture_coords.data());
	// colorambient
	glBufferSubData(GL_ARRAY_BUFFER, 3*sizeof(float)*positions.size()+3*sizeof(float)*normals.size()+2*sizeof(float)*texture_coords.size(), 3*sizeof(float)*colorambient.size(), colorambient.data());
	// color diffuse
	glBufferSubData(GL_ARRAY_BUFFER, 3*sizeof(float)*positions.size()+3*sizeof(float)*normals.size()+2*sizeof(float)*texture_coords.size()+3*sizeof(float)*colorambient.size(), 3*sizeof(float)*colordiffuse.size(), colordiffuse.data());
	// color specular
	glBufferSubData(GL_ARRAY_BUFFER, 3*sizeof(float)*positions.size()+3*sizeof(float)*normals.size()+2*sizeof(float)*texture_coords.size()+3*sizeof(float)*colorambient.size()*2, 3*sizeof(float)*colorspecular.size(), colorspecular.data());
	// shininess
	glBufferSubData(GL_ARRAY_BUFFER, 3*sizeof(float)*positions.size()+3*sizeof(float)*normals.size()+2*sizeof(float)*texture_coords.size()+3*sizeof(float)*colorambient.size()*3, sizeof(float)*colorshininess.size(), colorshininess.data());

	glGenBuffers(1, &(this->EBO));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->IndicesSize, this->Indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (GLvoid*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (GLvoid*)(3*sizeof(float)*positions.size()));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (GLvoid*)(3*sizeof(float)*positions.size()+3*sizeof(float)*normals.size()));
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (GLvoid*)(3*sizeof(float)*positions.size()+3*sizeof(float)*normals.size()+2*sizeof(float)*texture_coords.size()));
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (GLvoid*)(3*sizeof(float)*positions.size()+3*sizeof(float)*normals.size()+2*sizeof(float)*texture_coords.size()+3*sizeof(float)*colorambient.size()));
	glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (GLvoid*)(3*sizeof(float)*positions.size()+3*sizeof(float)*normals.size()+2*sizeof(float)*texture_coords.size()+3*sizeof(float)*colorambient.size()*2));
	glVertexAttribPointer(6, 1, GL_FLOAT, GL_FALSE,   sizeof(float), (GLvoid*)(3*sizeof(float)*positions.size()+3*sizeof(float)*normals.size()+2*sizeof(float)*texture_coords.size()+3*sizeof(float)*colorambient.size()*3));

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
