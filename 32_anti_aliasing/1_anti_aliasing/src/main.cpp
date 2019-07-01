/*****************************************************************************
 * Author : Ram
 * Date : 27/06/19
 * Email : ramkalath@gmail.com
 * Breif Description : Multi Sample Anti Aliasing
 * Detailed Description : antialiasing with opengl MSAA
 *****************************************************************************/
#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <iostream>
#include "SOIL.h"
#include "../include/shader.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <math.h>

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
	glfwWindowHint(GLFW_SAMPLES, 4);

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
	glEnable(GL_MULTISAMPLE);

	// Viewport dimensions
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, 800, 600);

	Shader our_shader("./shaders/vertex_shader.vert", "./shaders/fragment_shader.frag");
	// ================================================================================

	// data - vertices ===================================================================
	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.5f, -0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,  0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f, -0.5f, -0.5f, -0.5f,

		-0.5f, -0.5f,  0.5f,  0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f, -0.5f, -0.5f,  0.5f,

		-0.5f,  0.5f,  0.5f, -0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f, -0.5f,  0.5f,  0.5f,

		 0.5f,  0.5f,  0.5f,  0.5f,  0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,  0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,  0.5f,  0.5f,  0.5f,

		-0.5f, -0.5f, -0.5f,  0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,  0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f, -0.5f, -0.5f, -0.5f,

		-0.5f,  0.5f, -0.5f,  0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f, -0.5f,  0.5f, -0.5f
	};
	
	// ==================================================================================
	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
	
	glm::mat4 model = glm::mat4{1.0f};
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 1.0f));
	//model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));

	while(!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
        glClearColor(0.27f, 0.27f, 0.27f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
        glUseProgram(our_shader.program);
		glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 projection_perspective = glm::perspective(45.0f, (float)width/(float)height, 0.1f, 100.0f);
		glUniformMatrix4fv(glGetUniformLocation(our_shader.program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(glGetUniformLocation(our_shader.program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(our_shader.program, "projection_perspective"), 1, GL_FALSE, glm::value_ptr(projection_perspective));

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();
    return 0;
}

