/*****************************************************************************
 * Author : Ram
 * Date : 30/March/2019
 * Email : ramkalath@gmail.com
 * Breif Description : framebuffer testing
 * Detailed Description : 3d framebuffers
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

using namespace std;

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	// When the user presses the escape key, we set the window should close property to true, and close the application.
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

int width = 640, height = 480;

int main()
{
	// glfw stuff ====================================================================
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow *window = glfwCreateWindow(width, height, "View and perspective projection Matrices", nullptr, nullptr);
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
	glViewport(0, 0, width, height);

	Shader our_shader("./shaders/vertex_shader.vert", "./shaders/fragment_shader.frag");
	Shader fbplate_shader("./shaders/fbplate_vertex_shader.vert", "./shaders/fbplate_fragment_shader.frag");

	// =================================================================================
	float floor[] = {-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
					  1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
					  1.0f,  1.0f, 0.0f, 1.0f, 1.0f, 
					 -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
					 -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
					  1.0f,  1.0f, 0.0f, 1.0f, 1.0f};

	unsigned int VBO_floor, VAO_floor;
	glGenVertexArrays(1, &VAO_floor);
	glGenBuffers(1, &VBO_floor);
	glBindVertexArray(VAO_floor);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_floor);
	glBufferData(GL_ARRAY_BUFFER, sizeof(floor), floor, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (GLvoid*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (GLvoid*)(3*sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);

	unsigned int texture_floor;
	int width_texture, height_texture;
	glGenTextures(1, &texture_floor);
	glBindTexture(GL_TEXTURE_2D, texture_floor);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	unsigned char* image = SOIL_load_image("./resources/metal.jpg", &width_texture, &height_texture, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width_texture, height_texture, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// data - box ===================================================================
	float box[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0.5f, -0.5f, -0.5f,  1.0f, 0.0f,  0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.5f, -0.5f,  0.5f,  1.0f, 1.0f,  0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, -0.5f,  0.5f, -0.5f,  0.0f, 1.0f };

	unsigned int VBO_box, VAO_box;
	glGenVertexArrays(1, &VAO_box);
	glGenBuffers(1, &VBO_box);
	glBindVertexArray(VAO_box);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_box);
	glBufferData(GL_ARRAY_BUFFER, sizeof(box), box, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (GLvoid*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (GLvoid*)(3*sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);

	unsigned int texture_box;
	glGenTextures(1, &texture_box);
	glBindTexture(GL_TEXTURE_2D, texture_box);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	image = SOIL_load_image("./resources/wood.jpg", &width_texture, &height_texture, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width_texture, height_texture, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Model matrix
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, -0.5f, 1.0f));
	model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	// View matrix
	glm::vec3 camera_pos = glm::vec3(0.0f, 1.0f, 5.0f); 
	glm::mat4 view = glm::lookAt(camera_pos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	// Perspective projection matrix
	float angle = 45.0f, n = 0.2f, f = 20.0f;
	float ar = (float)width/(float)height;
	glm::mat4 projection_perspective = {1/(ar*tan(angle/2)), 0, 0, 0, 0, 1/tan(angle/2), 0, 0, 0, 0, -(f+n)/(f-n), -2*f*n/(f-n), 0, 0, -1, 0};
	projection_perspective = glm::transpose(projection_perspective);

	// rectangular plate =============================================================================
	float fb_plate[] = {-1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 
						 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
						 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
						-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
						 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
						-1.0f,  1.0f, 0.0f, 0.0f, 1.0f};

	unsigned int VBO_plate, VAO_plate;
	glGenVertexArrays(1, &VAO_plate);
	glGenBuffers(1, &VBO_plate);
	glBindVertexArray(VAO_plate);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_plate);
	glBufferData(GL_ARRAY_BUFFER, sizeof(fb_plate), fb_plate, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (GLvoid*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (GLvoid*)(3*sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);

	unsigned int fbo;
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	unsigned int texColorBuffer;
	glGenTextures(1, &texColorBuffer);
	glBindTexture(GL_TEXTURE_2D, texColorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glBindTexture(GL_TEXTURE_2D, 0);

	unsigned int rboDepthStencil;
	glGenRenderbuffers(1, &rboDepthStencil);
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepthStencil);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rboDepthStencil);
	//glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer, 0);

	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "\033[0;91m ERROR::FRAMEBUFFER:: Framebuffer is not complete \033[0m\n" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glm::mat4 model_plate = glm::mat4(1.0f);
	model_plate = glm::translate(model_plate, glm::vec3(0.0f, 0.0f, 1.0f));
	model_plate = glm::scale(model_plate, glm::vec3(2.0f, 2.0f, 0.0f));

	while(!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		glClearColor(0.27f, 0.27f, 0.27f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// ################################################################################################################################################################## 
		// This block is what is to be rendered inside the frame buffer
		glBindFramebuffer(GL_FRAMEBUFFER, fbo); 

		glClearColor(0.15f, 0.23f, 0.26f, 1.0f); // creating the turquoise blue color within the framebuffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(our_shader.program);
		glUniformMatrix4fv(glGetUniformLocation(our_shader.program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(our_shader.program, "projection_perspective"), 1, GL_FALSE, glm::value_ptr(projection_perspective));

		// floor render
		glBindVertexArray(VAO_floor);
		glUniformMatrix4fv(glGetUniformLocation(our_shader.program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(our_shader.program, "this_texture"), 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture_floor);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);

		// box render
		glBindVertexArray(VAO_box);
		glUniformMatrix4fv(glGetUniformLocation(our_shader.program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(our_shader.program, "this_texture"), 1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture_box);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		glBindFramebuffer(GL_FRAMEBUFFER, 0); 
		// ################################################################################################################################################################## 

		// framebuffer rendering
		glUseProgram(fbplate_shader.program);
		glBindVertexArray(VAO_plate);
		glUniformMatrix4fv(glGetUniformLocation(fbplate_shader.program, "model"), 1, GL_FALSE, glm::value_ptr(model_plate));
		glUniformMatrix4fv(glGetUniformLocation(fbplate_shader.program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(fbplate_shader.program, "projection_perspective"), 1, GL_FALSE, glm::value_ptr(projection_perspective));
		glUniform1i(glGetUniformLocation(fbplate_shader.program, "this_texture"), 2);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, texColorBuffer);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
	}
	
	// clean up
	glDeleteFramebuffers(1, &fbo);
	glDeleteVertexArrays(1, &VAO_box);
	glDeleteBuffers(1, &VBO_box);
	glDeleteVertexArrays(1, &VAO_floor);
	glDeleteBuffers(1, &VBO_floor);
	glDeleteVertexArrays(1, &VAO_plate);
	glDeleteBuffers(1, &VBO_plate);

	glfwTerminate();
	return 0;
}
