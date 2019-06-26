/*****************************************************************************
 * Author : Ram
 * Date : 24/June/2019
 * Email : ramkalath@gmail.com
 * Breif Description : uranus + asteroid model loading with assimp.
 * Detailed Description : uranus + asteroid model loading with assimp. With proper instancing. Model loading is done using recursion which usually creates a lot of vao's and its complicated to work with that. My idea is to use the old serial loading framework with its vao exposed so that we can add rotation (theta and rotation axis) and scaling.
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

	Shader objectshader("./shaders/object_vertex_shader.vert", "./shaders/object_fragment_shader.frag");
	Shader dustshader("./shaders/dust_vertex_shader.vert", "./shaders/dust_fragment_shader.frag");

	Modelloader uranus("/home/ram/Downloads/3d_models/solarsystem/Neptune/Neptune_v2_L3.123c6fe2b903-2de3-4b54-836a-dd427a10e972/13908_Neptune_V2_l3.obj");
	uranus.modelmatrix = glm::mat4(1.0f);
	float sat_pos_x = 0.0f, sat_pos_z = 0.0f;
	uranus.modelmatrix = glm::translate(uranus.modelmatrix, glm::vec3(sat_pos_x, 0.0f, sat_pos_z));
	uranus.modelmatrix = glm::scale(uranus.modelmatrix, glm::vec3(0.004f, 0.004f, 0.004f));

	GLfloat speck[] = {
		 0.0f,  0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f
	};

	int num_specks = 100;
	float scale[num_specks];
	glm::vec3 translations[num_specks];
	for(unsigned int i=0; i<num_specks; i++) {
		scale[i] = generate_rand_nos(0.05f, 0.1f);
		translations[i] = glm::vec3(generate_rand_nos(-10.0f, 10.0f), generate_rand_nos(-10.0f, 10.0f), -10.0f);
	}

	unsigned int VBO, VAO, scaleVBO, translationsVBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(speck), speck, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &scaleVBO);
	glBindBuffer(GL_ARRAY_BUFFER, scaleVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*num_specks, &scale[0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 1*sizeof(float), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribDivisor(1, 1);

	glGenBuffers(1, &translationsVBO);
	glBindBuffer(GL_ARRAY_BUFFER, translationsVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*num_specks, &translations[0], GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (GLvoid*)0);
	glEnableVertexAttribArray(2);
	glVertexAttribDivisor(2, 1);

	glBindVertexArray(0);

	while(!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		 // draw object ---------------------------------------------------------------
		glUseProgram(objectshader.program);

		 // Uniforms
		glUniformMatrix4fv(glGetUniformLocation(objectshader.program, "model"), 1, GL_FALSE, glm::value_ptr(uranus.modelmatrix));
		glUniformMatrix4fv(glGetUniformLocation(objectshader.program, "view"), 1, GL_FALSE, glm::value_ptr(globalsettings.view));
		glUniformMatrix4fv(glGetUniformLocation(objectshader.program, "projection"), 1, GL_FALSE, glm::value_ptr(globalsettings.projection_perspective));

		glUniform3f(glGetUniformLocation(objectshader.program, "LightDirection"), 10.0f, -10.0f, -10.0f);
		glUniform3f(glGetUniformLocation(objectshader.program, "LightAmbient"), 0.1f, 0.1f, 0.1f);
		glUniform3f(glGetUniformLocation(objectshader.program, "LightDiffuse"), 0.8f, 0.8f, 0.8f);
		glUniform3f(glGetUniformLocation(objectshader.program, "LightSpecular"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(objectshader.program, "CameraPosition"), 0.0f, 0.0f, 0.0f);

		// render uranus
		//uranus.Draw(objectshader);

		glUseProgram(dustshader.program);
		glUniformMatrix4fv(glGetUniformLocation(dustshader.program, "view"), 1, GL_FALSE, glm::value_ptr(globalsettings.view));
		glUniformMatrix4fv(glGetUniformLocation(dustshader.program, "projection_perspective"), 1, GL_FALSE, glm::value_ptr(globalsettings.projection_perspective));

		glBindVertexArray(VAO);
		glDrawArraysInstanced(GL_TRIANGLES, 0, 3, num_specks);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
	}

	glfwTerminate();
    return 0;
}
