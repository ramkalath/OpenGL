/*****************************************************************************
 * Author : Ram
 * Date : 14/March/2019
 * Email : ramkalath@gmail.com
 * Breif Description : Object outlining. 
 * Detailed Description : Working well
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

	Shader our_shader("./shaders/vertex_shader.vert", "./shaders/fragment_shader.frag");
	Shader outline_shader("./shaders/vertex_shader.vert", "./shaders/outline_fragment_shader.frag");

	// =================================================================================
	GLfloat floor[] = { -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
						 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
						 1.0f,  1.0f, 0.0f, 1.0f, 1.0f, 
						-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
						-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
						 1.0f,  1.0f, 0.0f, 1.0f, 1.0f};
	// -----------------------------------------
	GLuint VBO_floor, VAO_floor;
	glGenVertexArrays(1, &VAO_floor);
	glGenBuffers(1, &VBO_floor);
	glBindVertexArray(VAO_floor);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_floor);
	glBufferData(GL_ARRAY_BUFFER, sizeof(floor), floor, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), (GLvoid*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);
	// -----------------------------------------
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
	// -----------------------------------------
	glm::vec3 camera_pos = glm::vec3(1.0f);
	glm::mat4 model_floor = glm::mat4(1.0f);
	model_floor = glm::rotate(model_floor, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model_floor = glm::translate(model_floor, glm::vec3(0.0f, 0.0f, 1.001f));
	model_floor = glm::scale(model_floor, glm::vec3(2.5f, 2.5f, 0.0f));
	glm::mat4 view_floor = glm::mat4(1.0f);

	// data - box ===================================================================
	GLfloat box[] = {
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
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};
	//==========================
	GLuint VBO_box, VAO_box;
	glGenVertexArrays(1, &VAO_box);
	glGenBuffers(1, &VBO_box);
	glBindVertexArray(VAO_box);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_box);
	glBufferData(GL_ARRAY_BUFFER, sizeof(box), box, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), (GLvoid*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);
	//==========================
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
	//==========================
	glm::mat4 model_box = glm::mat4(1.0f);
	model_box = glm::translate(model_box, glm::vec3(0.0f, -0.5f, 1.0f));
	model_box = glm::rotate(model_box, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 view_box = glm::mat4(1.0f);
	GLfloat angle = 45.0f, n = 0.2f, f = 20.0f;
	GLfloat ar = (GLfloat)width/(GLfloat)height;
	glm::mat4 projection_perspective = {1/(ar*tan(angle/2)), 0, 0, 0, 0, 1/tan(angle/2), 0, 0, 0, 0, -(f+n)/(f-n), -2*f*n/(f-n), 0, 0, -1, 0};
	projection_perspective = glm::transpose(projection_perspective);
	// =================================================================================
	
	glm::mat4 model_box1 = glm::translate(model_box, glm::vec3(1.3f, 0.0f, -1.0f));
	glm::mat4 model_box2 = glm::scale(model_box, glm::vec3(1.1f, 1.1f, 1.1f));
	glm::mat4 model_box3 = glm::scale(model_box1, glm::vec3(1.1f, 1.1f, 1.1f));

	while(!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		glClearColor(0.27f, 0.27f, 0.27f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		float speed = 0.01;
		float time = glfwGetTime()*speed;
		double counter = ((double)((int)time%10) + time-int(time)-5)*72;

		camera_pos = glm::vec3(sin(counter)*5, 2.0f, cos(counter)*5); 
		view_floor = glm::lookAt(camera_pos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		view_box = glm::lookAt(camera_pos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		glUseProgram(our_shader.program);
		// floor render
		glBindVertexArray(VAO_floor);
		glUniformMatrix4fv(glGetUniformLocation(our_shader.program, "model"), 1, GL_FALSE, glm::value_ptr(model_floor));
		glUniformMatrix4fv(glGetUniformLocation(our_shader.program, "view"), 1, GL_FALSE, glm::value_ptr(view_floor));
		glUniformMatrix4fv(glGetUniformLocation(our_shader.program, "projection_perspective"), 1, GL_FALSE, glm::value_ptr(projection_perspective));
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture_floor);
		glUniform1i(glGetUniformLocation(our_shader.program, "this_texture"), 0);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);

		glEnable(GL_STENCIL_TEST);
		/* TODO: VERY IMPORTANT */
		// When you enable color mask, during drawing into the stencil buffer, the colored fragments do not appear. Thus the below two lines HAVE to be commented if you want the object draw to the stencil buffer to be rendered as well.
		//glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE); // prevent writing to color buffer 
		//glDepthMask(GL_FALSE); // prevent writing to the depth buffer
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		glStencilMask(0xFF);
		glClear(GL_STENCIL_BUFFER_BIT);
		
		// box render
		glBindVertexArray(VAO_box);
		glUniformMatrix4fv(glGetUniformLocation(our_shader.program, "model"), 1, GL_FALSE, glm::value_ptr(model_box));
		glUniformMatrix4fv(glGetUniformLocation(our_shader.program, "view"), 1, GL_FALSE, glm::value_ptr(view_box));
		glUniformMatrix4fv(glGetUniformLocation(our_shader.program, "projection_perspective"), 1, GL_FALSE, glm::value_ptr(projection_perspective));
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture_box);
		glUniform1i(glGetUniformLocation(our_shader.program, "this_texture"), 1);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glUniformMatrix4fv(glGetUniformLocation(our_shader.program, "model"), 1, GL_FALSE, glm::value_ptr(model_box1));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE); // Now enable the color buffer
		glDepthMask(GL_TRUE); // enable the depth buffer as well
		glStencilMask(0x00); // Lock the stencil buffer and prevent it from writing again

		// outline render
		glStencilFunc(GL_EQUAL, 0, 0xFF); // Any place which has ref value = 0 Let it pass
		glUseProgram(outline_shader.program);
		glBindVertexArray(VAO_box);
		glUniformMatrix4fv(glGetUniformLocation(outline_shader.program, "model"), 1, GL_FALSE, glm::value_ptr(model_box2));
		glUniformMatrix4fv(glGetUniformLocation(outline_shader.program, "view"), 1, GL_FALSE, glm::value_ptr(view_box));
		glUniformMatrix4fv(glGetUniformLocation(outline_shader.program, "projection_perspective"), 1, GL_FALSE, glm::value_ptr(projection_perspective));
		//glDisable(GL_DEPTH_TEST);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glUniformMatrix4fv(glGetUniformLocation(our_shader.program, "model"), 1, GL_FALSE, glm::value_ptr(model_box3));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		//glEnable(GL_DEPTH_TEST);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
	}
	glDeleteVertexArrays(1, &VAO_box);
	glDeleteBuffers(1, &VBO_box);

	glfwTerminate();
    return 0;
}
