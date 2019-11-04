/*****************************************************************************
* Filename : main.cpp
* Date : 30-9-2019
* Author : Ram
* Email : ramkalath@gmail.com
* Breif Description : Shadows
* Detailed Description : 1) TODO(ram): Render floor with proper texturing.
*								TODO(ram): yet to create the second object
* 						 2) TODO(ram): first pass for shadowing.
* 						 3) TODO(ram): second pass for shadowing.
*****************************************************************************/

// GLEW and GLFW includes
// Use this for ycm autocompletion
#include "use_glew.h"
#ifdef USE_GLEW
	#define GLEW_STATIC
    #include <GL/glew.h>
#else
    #define GL_GLEXT_PROTOTYPES
    #include <GL/gl.h>
    #include <GL/glext.h>
#endif
#include <GLFW/glfw3.h>
#include <SOIL/SOIL.h>

// glm includes
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other popular includes
#include <vector>
#include <iostream>
#include <math.h>

// User created headers
#include "../include/shader.h"
#include "../include/modelloader.h"
#include "../include/gamesettings.h"

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	// When the user presses the escape key, we set the window should close property to true, and close the application.
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

std::vector<float> populate_vertices(int no_of_tiles_width, int no_of_tiles_height) // segment floor width; segment floor height
{
	// TODO(ram): yet to fix the tex coordinates; copy the coordinates from positions
	float sfw = 1.0/no_of_tiles_width;
	float sfh = 1.0/no_of_tiles_height;
	std::vector<float> v;
	for(int h=0; h<no_of_tiles_width; h++) 
		for(int w=0; w<no_of_tiles_height; w++) 
		{
			//------------ positions ----------------------- | ---------------- normals ---------------------------- | ----- tex coords ---------------- |
			v.push_back(w*sfw); v.push_back(h*sfh); v.push_back(0.0f); v.push_back(0.0f); v.push_back(0.0f); v.push_back(1.0f); v.push_back(w*sfw); v.push_back(h*sfh);
			v.push_back((w+1)*sfw); v.push_back(h*sfh); v.push_back(0.0f); v.push_back(0.0f); v.push_back(0.0f); v.push_back(1.0f); v.push_back((w+1)*sfw); v.push_back(h*sfh);
			v.push_back((w+1)*sfw); v.push_back((h+1)*sfh); v.push_back(0.0f); v.push_back(0.0f); v.push_back(0.0f); v.push_back(1.0f); v.push_back((w+1)*sfw); v.push_back((h+1)*sfh);

			v.push_back(w*sfw); v.push_back(h*sfh); v.push_back(0.0f); v.push_back(0.0f); v.push_back(0.0f); v.push_back(1.0f); v.push_back(w*sfw); v.push_back(h*sfh);
			v.push_back((w+1)*sfw); v.push_back((h+1)*sfh); v.push_back(0.0f); v.push_back(0.0f); v.push_back(0.0f); v.push_back(1.0f); v.push_back((w+1)*sfw); v.push_back((h+1)*sfh);
			v.push_back(w*sfw); v.push_back((h+1)*sfh); v.push_back(0.0f); v.push_back(0.0f); v.push_back(0.0f); v.push_back(1.0f); v.push_back(w*sfw); v.push_back((h+1)*sfh);
		}
	return v;
}

int height_window = 640, width_window = 800;

int main()
{
	GameSettings globalsettings;
	// glfw stuff ====================================================================
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	GLFWwindow *window = glfwCreateWindow(width_window, height_window, "specular_lighting", nullptr, nullptr);
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);

	if(window == nullptr)
	{
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glewExperimental = GL_TRUE;

	if(glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}

	glEnable(GL_DEPTH_TEST);

	glfwGetFramebufferSize(window, &width_window, &height_window);
	glViewport(0, 0, width_window, height_window);

	Shader objectshader("./shaders/vertex_shader.vert", "./shaders/fragment_shader.frag");

	// defining floor vertices
	int no_of_tiles_width = 10, no_of_tiles_height = 10;
	std::vector<float> floor = populate_vertices(no_of_tiles_width, no_of_tiles_height);
	float *floor_vertices = &floor[0];
	int no_floor_floats = 6 * no_of_tiles_width * no_of_tiles_height * 8; // TODO(ram): what does that 6 do?

	unsigned int VBO_floor, VAO_floor;
	glGenVertexArrays(1, &VAO_floor);
	glGenBuffers(1, &VBO_floor);
	glBindVertexArray(VAO_floor);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_floor);
	glBufferData(GL_ARRAY_BUFFER, no_floor_floats*sizeof(float), floor_vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (GLvoid*)0); // positions
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat))); // normals
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (GLvoid*)(6*sizeof(GLfloat))); // textures
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);
	float theta = glm::radians(45.0f);
	glm::mat4 floor_model_matrix = glm::mat4{1.0f,  0.0f, 0.0f, 0.0f,
											 0.0f,  cos(theta), sin(theta), 0.0f,
											 0.0f,  -sin(theta), cos(theta), 0.0f,
											-0.5f, -0.5f, 0.0f, 1.0f}; // model matrix

	GLfloat near = 0.1f, far = 300.0f;
	GLfloat ar = (GLfloat)width_window/(GLfloat)height_window; // aspect ratio
	glm::mat4 projection_perspective = {1/(ar*tan(45/2)), 0, 0, 0,
										0, 1/tan(45/2), 0, 0,
										0, 0, -(far+near)/(far-near), -2*far*near/(far-near),
										0, 0, -1, 0};
	projection_perspective = glm::transpose(projection_perspective);

	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	
	int width_texture, height_texture;
	unsigned char* image = SOIL_load_image("./resources/brick_texture.jpg", &width_texture, &height_texture, 0, SOIL_LOAD_RGB);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width_texture, height_texture, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);


	while(!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		glClearColor(0.27f, 0.27f, 0.27f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		float time = sin(glfwGetTime()) * 10.0f;
		//std::cout << time << std::endl;
		glm::mat4 view_matrix = glm::lookAt(glm::vec3(0.0f, 30.0f, 80.0f), 
											glm::vec3(0.0f, 0.0f, 0.0f), 
											glm::vec3(0.0f, 1.0f, 0.0f));

		glUseProgram(objectshader.program);

		// Activate Texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glUniform1i(glGetUniformLocation(objectshader.program, "my_texture"), 0);

		// Upload Geometric Uniforms
		glBindVertexArray(VAO_floor);
		glUniformMatrix4fv(glGetUniformLocation(objectshader.program, "model"), 1, GL_FALSE, glm::value_ptr(floor_model_matrix));
		glUniformMatrix4fv(glGetUniformLocation(objectshader.program, "view"), 1, GL_FALSE, glm::value_ptr(view_matrix));
		glUniformMatrix4fv(glGetUniformLocation(objectshader.program, "projection"), 1, GL_FALSE, glm::value_ptr(projection_perspective));

		// Draw call
		//glPolygonMode( GL_FRONT_AND_BACK, GL_LINE ); // Draw wireframe
		glDrawArrays(GL_TRIANGLES, 0, no_floor_floats); // draw call

		glBindVertexArray(0);

		glfwSwapBuffers(window);
	}
	glfwTerminate();
	return 0;
}
