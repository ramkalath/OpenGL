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
#include <stdlib.h>
#include <time.h>
#include <cmath>
#include <string>
#include <sstream>

// User created headers
#include "../include/shader.h"
#include "../include/modelloader.h"
#include "../include/gamesettings.h"

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode) {
	// When the user presses the escape key, we set the window should close property to true, and close the application.
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

float generate_rand_nos(float min, float max) {
	int precision = 1000; 
	return min+((max-min)/precision*(std::rand()%precision+1));
}

glm::vec3 calculate_random_position(float radius, float center_x, float center_z) {
	int theta = std::rand()%360;
	float random_x = 1-((float)std::rand()/RAND_MAX)*2;
	float random_z = 1-((float)std::rand()/RAND_MAX)*2;
	glm::vec3 pos = glm::vec3(radius*cos(theta)+center_x+random_x, -1.0f, radius*sin(theta)+center_z+random_z);
	return pos;
}

int width = 1500, height = 1000;

int main()
{
	float counter=0; 
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
	saturn.modelmatrix = glm::mat4(1.0f);
	float sat_pos_x = 2.0f, sat_pos_z = -2.0f;
	saturn.modelmatrix = glm::translate(saturn.modelmatrix, glm::vec3(sat_pos_x, -1.0f, sat_pos_z));
	saturn.modelmatrix = glm::scale(saturn.modelmatrix, glm::vec3(0.005f, 0.005f, 0.005f));
	saturn.modelmatrix = glm::rotate(saturn.modelmatrix, glm::radians(-25.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	saturn.modelmatrix = glm::rotate(saturn.modelmatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	Modelloader asteroid("./resources/stone/stone.obj");
	asteroid.modelmatrix = glm::mat4(1.0f);
	float scale = ((float)std::rand()/RAND_MAX);
	asteroid.modelmatrix = glm::scale(asteroid.modelmatrix, glm::vec3(0.1f*scale, 0.2f*scale, 0.1f*scale));

	glm::vec3 rot_axis[100];
	for(unsigned int i=0; i<100; i++)
		rot_axis[i] = glm::vec3(generate_rand_nos(-0.9f, 0.9f), generate_rand_nos(-0.9f, 0.9f), generate_rand_nos(-0.9f, 0.9f));

	unsigned int num_asteroids = 500;
	glm::mat4 model_asteroid[num_asteroids];

	for(unsigned int i=0; i<num_asteroids; i++) {
		model_asteroid[i] = glm::mat4(1.0f);
		srand(i);
		float theta = generate_rand_nos(0.1f, 10.0f);
		float radius = 15.0f;
		glm::vec3 pos = glm::vec3(radius*cos(theta)+sat_pos_x+-2.0f+generate_rand_nos(-2.0f, 2.0f), -1.0f, radius*sin(theta)+sat_pos_z-10.0f+generate_rand_nos(-2.0f, 2.0f));
		model_asteroid[i] = glm::translate(model_asteroid[i], pos);
		model_asteroid[i] = glm::scale(model_asteroid[i], glm::vec3(generate_rand_nos(0.01, 0.3), generate_rand_nos(0.01, 0.3), generate_rand_nos(0.01, 0.3)));
	}

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
		glUniform3f(glGetUniformLocation(objectshader.program, "LightDirection"), 10.0f, -10.0f, -10.0f);
		glUniform3f(glGetUniformLocation(objectshader.program, "LightAmbient"), 0.1f, 0.1f, 0.1f);
		glUniform3f(glGetUniformLocation(objectshader.program, "LightDiffuse"), 0.8f, 0.8f, 0.8f);
		glUniform3f(glGetUniformLocation(objectshader.program, "LightSpecular"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(objectshader.program, "CameraPosition"), 0.0f, 0.0f, 0.0f);

		// render saturn
		glUniformMatrix4fv(glGetUniformLocation(objectshader.program, "model"), 1, GL_FALSE, glm::value_ptr(saturn.modelmatrix));
		saturn.Draw(objectshader);



		counter=1;
		for(unsigned int i=0; i<num_asteroids; i++)
		{
			std::stringstream index;
			index << i;
			model_asteroid[i] = glm::rotate(model_asteroid[i], glm::radians(1.0f), rot_axis[i]);
			glUniformMatrix4fv(glGetUniformLocation(objectshader.program, ("model[" + index.str()+"]").c_str()), 1, GL_FALSE, glm::value_ptr(model_asteroid[i]));
		}
		asteroid.DrawInstanced(objectshader, 100);

		glfwSwapBuffers(window);
	}

	glfwTerminate();
    return 0;
}
