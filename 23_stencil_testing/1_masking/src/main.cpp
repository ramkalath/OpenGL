/*****************************************************************************
 * Author : Ram
 * Date : 12/March/2019
 * Email : ramkalath@gmail.com
 * Breif Description : stencil test Masking the below rectangle with a triangle
 * Detailed Description : there is a redish rectangle behind and a triangular stencil. I pretty much want only the area within the triangle to emit the contents behind. The area outside the triangle is masked. Its like looking through onto a red wall paper with a triangulare hole cut on a piece of paper.
 *****************************************************************************/
#ifdef CLANG_COMPLETE_ONLY
    #define GL_GLEXT_PROTOTYPES
    #include <GL/gl.h>
    #include <GL/glext.h>
#else
    #include <GL/glew.h>
#endif

// GLFW
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
	glDepthFunc(GL_LESS);

	// Viewport dimensions
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, 800, 600);

	Shader simple_shader("./shaders/vertex_shader.vert", "./shaders/fragment_shader.frag");

	// lets draw a simple rectangle ----------------------------------------------------------
	float rect[] ={ -0.5f, -0.5f, 0.001f,
					 0.5f, -0.5f, 0.001f,
					 0.5f,  0.5f, 0.001f,
					-0.5f, -0.5f, 0.001f,
					-0.5f,  0.5f, 0.001f,
					 0.5f,  0.5f, 0.001f};

	GLuint VBO_rect, VAO_rect;
	glGenVertexArrays(1, &VAO_rect);
	glGenBuffers(1, &VBO_rect);
	glBindVertexArray(VAO_rect);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_rect);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rect), rect, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	// lets draw a simple triangle ----------------------------------------------------------
	float tri[] ={ -0.5f, -0.5f, 0.0f,
					0.5f, -0.5f, 0.0f,
					0.0f,  0.5f, 0.0f};

	GLuint VBO_tri, VAO_tri;
	glGenVertexArrays(1, &VAO_tri);
	glGenBuffers(1, &VBO_tri);
	glBindVertexArray(VAO_tri);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_tri);
	glBufferData(GL_ARRAY_BUFFER, sizeof(tri), tri, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	while(!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		// settings
		glEnable(GL_STENCIL_TEST); // enable stencil test
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE); // prevent writing to color buffer
		glDepthMask(GL_FALSE); // prevent writing to the depth buffer
		glStencilFunc(GL_NEVER, 1, 0xFF); // Stencil test always fails
		glStencilOp(GL_REPLACE, GL_KEEP, GL_KEEP); // Replace the stencil buffer with the ref value -> here its equal to 1
		glStencilMask(0xFF); // Now we just set all the bits of the stencil mask to 1 so that its writable

		// the below block draws a stencil pattern of a triangle
		// at this point anything we write gets written to the stencil buffer
		glUseProgram(simple_shader.program);
		glBindVertexArray(VAO_tri);
		glUniform3f(glGetUniformLocation(simple_shader.program, "color_value"), 0.5f, 0.5f, 0.5f);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);

		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE); // Now enable the color buffer
		glDepthMask(GL_TRUE); // enable the depth buffer as well
		glStencilMask(0x00); // Lock the stencil buffer and prevent it from writing again
		glStencilFunc(GL_EQUAL, 1, 0xFF); // Any place which has ref value = 1 Let it pass

		// Now simply paint the color buffer. Only the contents that have stencil buffer value = 1 will pass through
		glUseProgram(simple_shader.program);
		glBindVertexArray(VAO_rect);
		glUniform3f(glGetUniformLocation(simple_shader.program, "color_value"), 0.8f, 0.2f, 0.2f);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);

		glDisable(GL_STENCIL_TEST); // Finally disable the stencil test

		glfwSwapBuffers(window);
	}

	glfwTerminate();
    return 0;
}
