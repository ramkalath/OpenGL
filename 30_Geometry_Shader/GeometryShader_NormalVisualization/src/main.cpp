/*****************************************************************************
 * Author : Ram
 * Date : 21/Feb/2019
 * Email : ramkalath@gmail.com
 * Breif Description : nanosuit model loading with assimp.
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
#include "../include/shader2.h"
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

	Shader os("./shaders/vertex_shader1.vert", "./shaders/fragment_shader1.frag");
	Normalshader ns("./shaders/vertex_shader2.vert", "./shaders/geometry_shader2.geom" ,"./shaders/fragment_shader2.frag");

	Modelloader nanosuit("./resources/nanosuit/nanosuit.obj");


	nanosuit.modelmatrix = {1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f};
	nanosuit.modelmatrix = glm::translate(nanosuit.modelmatrix, glm::vec3(0.0f, -2.0f, -2.0f));
	nanosuit.modelmatrix = glm::scale(nanosuit.modelmatrix, glm::vec3(0.3f, 0.3f, 0.3f));

	while(!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		 // draw object ---------------------------------------------------------------
		glUseProgram(os.program);
		glUniformMatrix4fv(glGetUniformLocation(os.program, "model"), 1, GL_FALSE, glm::value_ptr(nanosuit.modelmatrix));
		glUniformMatrix4fv(glGetUniformLocation(os.program, "view"), 1, GL_FALSE, glm::value_ptr(globalsettings.view));
		glUniformMatrix4fv(glGetUniformLocation(os.program, "projection"), 1, GL_FALSE, glm::value_ptr(globalsettings.projection_perspective));
		glUniform3f(glGetUniformLocation(os.program, "LightDirection"), 10.0f, 10.0f, -10.0f);
		glUniform3f(glGetUniformLocation(os.program, "LightAmbient"), 0.5f, 0.5f, 0.5f);
		glUniform3f(glGetUniformLocation(os.program, "LightDiffuse"), 1.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(os.program, "LightSpecular"), 1.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(os.program, "CameraPosition"), 0.0f, 0.0f, 0.0f);
		nanosuit.Draw(os);

		//glUseProgram(ns.program);
		//glUniformMatrix4fv(glGetUniformLocation(ns.program, "model"), 1, GL_FALSE, glm::value_ptr(nanosuit.modelmatrix));
		//glUniformMatrix4fv(glGetUniformLocation(ns.program, "view"), 1, GL_FALSE, glm::value_ptr(globalsettings.view));
		//glUniformMatrix4fv(glGetUniformLocation(ns.program, "projection"), 1, GL_FALSE, glm::value_ptr(globalsettings.projection_perspective));
		//nanosuit.Draw(ns);

		glfwSwapBuffers(window);
	}

	glfwTerminate();
    return 0;
}
