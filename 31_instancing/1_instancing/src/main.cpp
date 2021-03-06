/*****************************************************************************
 * Author : Ram
 * Date : 8/June/2019
 * Email : ramkalath@gmail.com
 * Breif Description : Instancing
 * Detailed Description : instancing
 *****************************************************************************/
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

#include <iostream>
#include "SOIL.h"
#include "../include/shader.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <math.h>
#include <string>
#include <sstream>

using namespace std;

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	// When the user presses the escape key, we set the window should close property to true, and close the application.
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

int width = 640, height = 800;

int main()
{
	// glfw stuff ====================================================================
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow *window = glfwCreateWindow(800, 600, "View and perspective projection Matrices", nullptr, nullptr);
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

	// Viewport dimensions
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, 800, 600);

	Shader our_shader("./shaders/vertex_shader.vert", "./shaders/fragment_shader.frag");
	// ================================================================================

	// data - vertices ===================================================================
	float vertices[] = {
		-0.05f,  0.05f, 1.0f, 0.0f, 0.0f,
		 0.05f, -0.05f, 0.0f, 1.0f, 0.0f,
		-0.05f, -0.05f, 0.0f, 0.0f, 1.0f,

		-0.05f,  0.05f, 1.0f, 0.0f, 0.0f,
		 0.05f, -0.05f, 0.0f, 1.0f, 0.0f,
		 0.05f,  0.05f, 0.0f, 1.0f, 1.0f
	};
	
	// ==================================================================================
	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (GLvoid*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (GLvoid*)(2*sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindVertexArray(0); // Unbind VAO
	
	glm::vec2 translations[100];
	int index = 0;
	float offset = 0.1f;
	for(int i = -10; i < 10; i+=2) {
		for(int j = -10; j < 10; j+=2) {
			glm::vec2 translation;
			translation.x = (float)i/10.0f + offset;
			translation.y = (float)j/10.0f + offset;
			translations[index++]=translation;
		}
	}
	
	while(!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

        glClearColor(0.27f, 0.27f, 0.27f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
        glUseProgram(our_shader.program);

		for(unsigned int i=0; i<100; i++)
		{
			std::stringstream index;
			index << i;
			glUniform2f(glGetUniformLocation(our_shader.program, ("offsets[" + index.str()+"]").c_str()), translations[i].x, translations[i].y); 
		}
		
		glBindVertexArray(VAO);
		glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 100);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();
    return 0;
}
