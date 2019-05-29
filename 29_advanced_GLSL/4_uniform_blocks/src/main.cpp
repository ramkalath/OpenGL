/*****************************************************************************
 * Author : Ram
 * Date : 29/May/2019
 * Email : ramkalath@gmail.com
 * Breif Description : Uniform buffers with opengl
 * Detailed Description : Not completed. Lots of gaps in concept understanding. Yet to read and understand from various other sources
 *****************************************************************************/

#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

#include <iostream>
#include "../include/shader.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <math.h>
#include <SOIL/SOIL.h>

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

	Shader shaderRed("./shaders/vertex_shader.vert", "./shaders/fragment_shader_red.frag");
	Shader shaderGreen("./shaders/vertex_shader.vert", "./shaders/fragment_shader_green.frag");
	Shader shaderBlue("./shaders/vertex_shader.vert", "./shaders/fragment_shader_blue.frag");
	Shader shaderYellow("./shaders/vertex_shader.vert", "./shaders/fragment_shader_yellow.frag");
	// ================================================================================

	// data - vertices ===================================================================
	GLfloat vertices[] = 
 	{
    	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
    	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
    	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f, -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
   		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
    	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f, -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};
	
	// ==================================================================================
	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0); // Unbind VAO
	
	// ==================================================================================
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-1.0f, 1.0f, 0.0f));
	glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	GLfloat angle = 45.0f, n = 0.1f, f = 100.0f, ar = (GLfloat)width/(GLfloat)height;
	glm::mat4 projection_perspective = {1/(ar*tan(angle/2)), 0, 0, 0,
										0, 1/tan(angle/2), 0, 0,
										0, 0, -(f+n)/(f-n), -2*f*n/(f-n),
										0, 0, -1, 0};
	projection_perspective = glm::transpose(projection_perspective);

	unsigned int uniformBlockIndexRed = glGetUniformBlockIndex(shaderRed.program, "Matrices");
	unsigned int uniformBlockIndexGreen = glGetUniformBlockIndex(shaderGreen.program, "Matrices");
	unsigned int uniformBlockIndexBlue = glGetUniformBlockIndex(shaderBlue.program, "Matrices");
	unsigned int uniformBlockIndexYellow = glGetUniformBlockIndex(shaderYellow.program, "Matrices");

    glUniformBlockBinding(shaderRed.program, uniformBlockIndexRed, 0);
    glUniformBlockBinding(shaderGreen.program, uniformBlockIndexGreen, 0);
    glUniformBlockBinding(shaderBlue.program, uniformBlockIndexBlue, 0);
    glUniformBlockBinding(shaderYellow.program, uniformBlockIndexYellow, 0);

    unsigned int uboMatrices;
    glGenBuffers(1, &uboMatrices);
    glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
    glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    // Define the range of the buffer that links to a uniform binding point
    glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboMatrices, 0, 2 * sizeof(glm::mat4));
    glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projection_perspective));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

	while(!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

        glClearColor(0.27f, 0.27f, 0.27f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
        // Set the view and projection matrix in the uniform block - we only have to do this once per loop iteration
        glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
        glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));
        glBindBuffer(GL_UNIFORM_BUFFER, 0);

        glBindVertexArray(VAO);
		
		// RED
        glUseProgram(shaderRed.program);
        glm::mat4 model;
        model = glm::translate(model, glm::vec3(-1.0f, 1.0f, 0.0f)); // Move top-left
        glUniformMatrix4fv(glGetUniformLocation(shaderRed.program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);        
        // GREEN
        glUseProgram(shaderGreen.program);
        model = glm::mat4();
        model = glm::translate(model, glm::vec3(1.0f, 1.0f, 0.0f)); // Move top-right
        glUniformMatrix4fv(glGetUniformLocation(shaderGreen.program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);        
        // BLUE
        glUseProgram(shaderBlue.program);
        model = glm::mat4();
        model = glm::translate(model, glm::vec3(-1.0f, -1.0f, 0.0f)); // Move bottom-left
        glUniformMatrix4fv(glGetUniformLocation(shaderBlue.program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);        
        // YELLOW
        glUseProgram(shaderYellow.program);
        model = glm::mat4();
        model = glm::translate(model, glm::vec3(1.0f, -1.0f, 0.0f)); // Move bottom-right
        glUniformMatrix4fv(glGetUniformLocation(shaderYellow.program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);        

        glBindVertexArray(0);	
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
