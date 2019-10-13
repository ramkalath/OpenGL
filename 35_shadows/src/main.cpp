/*****************************************************************************
* Filename : main.cpp
* Date : 30-9-2019
* Author : Ram
* Email : ramkalath@gmail.com
* Breif Description : Shadows
* Detailed Description : 1) DONE: step 1 is to generate a directional light over two plates. One hovering over the other.
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

// glm includes
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other popular includes
#include <iostream>
#include <vector>

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

std::vector<float> populate_vertices(unsigned int height, unsigned int width)
{
	std::vector<float> v;
	for(unsigned int h=0; h<height; h++) 
		for(unsigned int w=0; w<width; w++) {
			v.push_back(w); v.push_back(h); v.push_back(0.0f); v.push_back(0.0f); v.push_back(0.0f); v.push_back(1.0f); v.push_back(w); v.push_back(h);
			v.push_back(w+1); v.push_back(h); v.push_back(0.0f); v.push_back(0.0f); v.push_back(0.0f); v.push_back(1.0f); v.push_back(w+1); v.push_back(h);
			v.push_back(w); v.push_back(h+1); v.push_back(0.0f); v.push_back(0.0f); v.push_back(0.0f); v.push_back(1.0f); v.push_back(w); v.push_back(h+1);

			v.push_back(w+1); v.push_back(h); v.push_back(0.0f); v.push_back(0.0f); v.push_back(0.0f); v.push_back(1.0f); v.push_back(w+1); v.push_back(h);
			v.push_back(w+1); v.push_back(h+1); v.push_back(0.0f); v.push_back(0.0f); v.push_back(0.0f); v.push_back(1.0f); v.push_back(w+1); v.push_back(h);
			v.push_back(w); v.push_back(h+1); v.push_back(0.0f); v.push_back(0.0f); v.push_back(0.0f); v.push_back(1.0f); v.push_back(w); v.push_back(h+1);
		}
	return v;
}

int width = 640, height = 800;

int main()
{
	GameSettings globalsettings;
	// glfw stuff ====================================================================
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    GLFWwindow *window = glfwCreateWindow(800, 600, "specular_lighting", nullptr, nullptr);
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

	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	Shader objectshader("./shaders/vertex_shader.vert", "./shaders/fragment_shader.frag");
	Shader shadowshader("./shaders/depth_vertex_shader.vert", "./shaders/depth_fragment_shader.frag");

	// lets define table and plate on our own
	unsigned int height = 10;
	unsigned int width = 10;
	std::vector<float> floor = populate_vertices(10, 10);
	float *floor_vertices = &floor[0];
	int no_floor_floats = 6 * height * width * 8;

	GLuint VBO_floor, VAO_floor;
	glGenVertexArrays(1, &VAO_floor);
	glGenBuffers(1, &VBO_floor);
	glBindVertexArray(VAO_floor);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_floor);
	glBufferData(GL_ARRAY_BUFFER, no_floor_floats*sizeof(float), floor_vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (GLvoid*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat)));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (GLvoid*)(2*sizeof(GLfloat)));
	glEnableVertexAttribArray(0);
	//glEnableVertexAttribArray(1);
	//glEnableVertexAttribArray(2);
	glBindVertexArray(0);
	glm::mat4 floor_model_matrix = glm::mat4{1.0f};


	// Lets define a depth texture for the shadows
	unsigned int FramebufferName = 0;
	glGenFramebuffers(1, &FramebufferName);
	glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);

	unsigned depthTexture;
	glGenTextures(1, &depthTexture);
	glBindTexture(GL_TEXTURE_2D, depthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTexture, 0);

	glDrawBuffer(GL_NONE);

	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		return 0;

	while(!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

        glClearColor(0.27f, 0.27f, 0.27f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		glUseProgram(shadowshader.program);

		// Uniforms
		glUniformMatrix4fv(glGetUniformLocation(objectshader.program, "model"), 1, GL_FALSE, glm::value_ptr(floor_model_matrix));
		glUniformMatrix4fv(glGetUniformLocation(objectshader.program, "view"), 1, GL_FALSE, glm::value_ptr(globalsettings.view));
		glUniformMatrix4fv(glGetUniformLocation(objectshader.program, "projection"), 1, GL_FALSE, glm::value_ptr(globalsettings.projection_perspective));

		// Draw the item
		glDrawArrays(GL_TRIANGLES, 0, no_floor_floats);

		glfwSwapBuffers(window);
	}
	glfwTerminate();
	return 0;
}
