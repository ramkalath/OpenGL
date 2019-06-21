/*****************************************************************************
 * Author : Ram
 * Date : 21/Feb/2019
 * Email : ramkalath@gmail.com
 * Breif Description : saturn model loading with assimp.
 * Detailed Description : object loading with assimp with directional light
 *****************************************************************************/
// GLEW and GLFW includes
#define GLEW_STATIC
#include <GL/glew.h>
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

int width = 1500, height = 1000;

int main()
{
	GameSettings globalsettings;
	// glfw stuff ====================================================================
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow *window = glfwCreateWindow(width, height, "Lighting Properties", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);

    if(window == nullptr) {
    	std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
    }
	glewExperimental = GL_TRUE;
	if(glewInit() != GLEW_OK) {
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}

	glEnable(GL_DEPTH_TEST);

	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	Shader objectshader("./shaders/vertex_shader.vert", "./shaders/fragment_shader.frag");

	Modelloader saturn("./resources/saturn/13906_Saturn_v1_l3.obj");
	//Modelloader saturn("/home/ram/Downloads/3d_models/solarsystem/saturn/13906_Saturn_v1_l3.obj");
	saturn.modelmatrix = glm::mat4(1.0f);
	saturn.modelmatrix = glm::translate(saturn.modelmatrix, glm::vec3(2.0f, -1.0f, -2.0f));
	saturn.modelmatrix = glm::scale(saturn.modelmatrix, glm::vec3(0.005f, 0.005f, 0.005f));
	saturn.modelmatrix = glm::rotate(saturn.modelmatrix, glm::radians(-25.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	saturn.modelmatrix = glm::rotate(saturn.modelmatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));


	Modelloader asteroid("./resources/asteroid/10464_Asteroid_v1_Iterations-2.obj");
	asteroid.modelmatrix = glm::mat4(1.0f);
	asteroid.modelmatrix = glm::translate(asteroid.modelmatrix, glm::vec3(-1.0f, -1.0f, -2.0f));
	asteroid.modelmatrix = glm::scale(asteroid.modelmatrix, glm::vec3(0.0005f, 0.0005f, 0.0005f));
	asteroid.modelmatrix = glm::rotate(asteroid.modelmatrix, glm::radians(-25.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	asteroid.modelmatrix = glm::rotate(asteroid.modelmatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	while(!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		 // draw object ---------------------------------------------------------------
		glUseProgram(objectshader.program);

		 // Uniforms
		glUniformMatrix4fv(glGetUniformLocation(objectshader.program, "view"), 1, GL_FALSE, glm::value_ptr(globalsettings.view));
		glUniformMatrix4fv(glGetUniformLocation(objectshader.program, "projection"), 1, GL_FALSE, glm::value_ptr(globalsettings.projection_perspective));
		glUniform3f(glGetUniformLocation(objectshader.program, "LightDirection"), 10.0f, 10.0f, -10.0f);
		glUniform3f(glGetUniformLocation(objectshader.program, "LightAmbient"), 0.5f, 0.5f, 0.5f);
		glUniform3f(glGetUniformLocation(objectshader.program, "LightDiffuse"), 1.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(objectshader.program, "LightSpecular"), 1.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(objectshader.program, "CameraPosition"), 0.0f, 0.0f, 0.0f);

		// render saturn
		glUniformMatrix4fv(glGetUniformLocation(objectshader.program, "model"), 1, GL_FALSE, glm::value_ptr(saturn.modelmatrix));
		saturn.Draw(objectshader);


		glUniformMatrix4fv(glGetUniformLocation(objectshader.program, "model"), 1, GL_FALSE, glm::value_ptr(asteroid.modelmatrix));
		asteroid.Draw(objectshader);

		glfwSwapBuffers(window);
	}

	glfwTerminate();
    return 0;
}
