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

	Modelloader table("./resources/table.obj");
	table.modelmatrix = glm::mat4{1.0f};
	table.modelmatrix = glm::translate(table.modelmatrix, glm::vec3(0.0f, -2.0f, 0.0f));
	table.modelmatrix = glm::scale(table.modelmatrix, glm::vec3(0.3f, 0.3f, 0.3f));

	Modelloader plate("./resources/plastic_top.obj");
	plate.modelmatrix = glm::mat4{1.0f};
	plate.modelmatrix = glm::translate(plate.modelmatrix, glm::vec3(0.0f, -1.0f, 0.0f));
	plate.modelmatrix = glm::scale(plate.modelmatrix, glm::vec3(0.3f, 0.3f, 0.3f));

	while(!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

        glClearColor(0.27f, 0.27f, 0.27f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		glUseProgram(objectshader.program);

		// Uniforms
		glUniformMatrix4fv(glGetUniformLocation(objectshader.program, "view"), 1, GL_FALSE, glm::value_ptr(globalsettings.view));
		glUniformMatrix4fv(glGetUniformLocation(objectshader.program, "projection"), 1, GL_FALSE, glm::value_ptr(globalsettings.projection_perspective));
		glUniform3f(glGetUniformLocation(objectshader.program, "LightDirection"), 0.0f, 5.0f, 0.0f);
		glUniform3f(glGetUniformLocation(objectshader.program, "LightAmbient"), 0.5f, 0.5f, 0.5f);
		glUniform3f(glGetUniformLocation(objectshader.program, "LightDiffuse"), 1.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(objectshader.program, "LightSpecular"), 1.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(objectshader.program, "CameraPosition"), 0.0f, 0.0f, 0.0f);

		// render table
		glUniformMatrix4fv(glGetUniformLocation(objectshader.program, "model"), 1, GL_FALSE, glm::value_ptr(table.modelmatrix));
		table.Draw(objectshader);

		// render plate
		glUniformMatrix4fv(glGetUniformLocation(objectshader.program, "model"), 1, GL_FALSE, glm::value_ptr(plate.modelmatrix));
		plate.Draw(objectshader);

		glfwSwapBuffers(window);
	}
	glfwTerminate();
	return 0;
}
