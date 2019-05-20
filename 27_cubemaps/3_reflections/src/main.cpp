/*****************************************************************************
 * Author : Ram
 * Date : 6/April/2019
 * Email : ramkalath@gmail.com
 * Breif Description : reflections
 * Detailed Description : simulates reflection on a cube
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
#include <vector>

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
	Shader skybox_shader("./shaders/skybox_vertex_shader.vert", "./shaders/skybox_fragment_shader.frag");

	// data - box ===================================================================
	float box[] = {
					-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
					 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
					-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 

					-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f, 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
					 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f, 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
					-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f, -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

					-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
					-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
					-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f, -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

					 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
					 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
					 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

					-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
					 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
					-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

					-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
					 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
					-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
				};

	GLuint VBO_box, VAO_box;
	glGenVertexArrays(1, &VAO_box);
	glGenBuffers(1, &VBO_box);
	glBindVertexArray(VAO_box);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_box);
	glBufferData(GL_ARRAY_BUFFER, sizeof(box), box, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat), (GLvoid*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);

	unsigned int texture_box;
	int width_texture, height_texture;
	glGenTextures(1, &texture_box);
	glBindTexture(GL_TEXTURE_2D, texture_box);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	unsigned char *image = SOIL_load_image("./resources/wood.jpg", &width_texture, &height_texture, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width_texture, height_texture, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	glm::mat4 model_box = glm::mat4(1.0f);
	model_box = glm::translate(model_box, glm::vec3(0.0f, -0.5f, 1.0f));
	model_box = glm::rotate(model_box, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 model_box1 = glm::translate(model_box, glm::vec3(1.3f, 0.0f, -1.0f));
	
	// =================================================================================
	// Cube Maps
	 float skyboxVertices[] = { -1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f, -1.0f, -1.0f,
								 1.0f, -1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f,  1.0f, -1.0f, 
								-1.0f, -1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f, -1.0f,
								-1.0f,  1.0f, -1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f,  1.0f,
								 1.0f, -1.0f, -1.0f,  1.0f, -1.0f,  1.0f,  1.0f,  1.0f,  1.0f,
								 1.0f,  1.0f,  1.0f,  1.0f,  1.0f, -1.0f,  1.0f, -1.0f, -1.0f,
								-1.0f, -1.0f,  1.0f, -1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f,
								 1.0f,  1.0f,  1.0f,  1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f,
								-1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f,  1.0f,
								 1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f, -1.0f,
								-1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f,
								 1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f };

    GLuint skyboxVAO, skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glBindVertexArray(0);

	std::vector<const char*> faces;
	faces.push_back("./resources/skybox/right.jpg");
	faces.push_back("./resources/skybox/left.jpg");
	faces.push_back("./resources/skybox/top.jpg");
	faces.push_back("./resources/skybox/bottom.jpg");
	faces.push_back("./resources/skybox/back.jpg");
	faces.push_back("./resources/skybox/front.jpg");

	unsigned int cubemapTexture;
	glGenTextures(1, &cubemapTexture);
	glBindTexture(GL_TEXTURE_2D, cubemapTexture);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	for(unsigned int i=0; i<faces.size(); i++) 
	{
		image = SOIL_load_image(faces[i], &width_texture, &height_texture, 0, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i, 0, GL_RGB, width_texture, height_texture, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	}
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	
	// skybox scale
	float skybox_scale = 50.0f;
	glm::mat4 model_skybox = glm::mat4(1.0f);
	model_skybox = glm::scale(model_skybox, glm::vec3(skybox_scale, skybox_scale, skybox_scale));
	// =================================================================================

	// perspective projection matrix
	float angle=45.0f, n=0.2f, f=100.0f, ar=(float)width/(float)height;
	glm::mat4 projection = {1/(ar*tan(angle/2)), 0, 0, 0, 0, 1/tan(angle/2), 0, 0, 0, 0, -(f+n)/(f-n), -2*f*n/(f-n), 0, 0, -1, 0};
	projection = glm::transpose(projection);

	while(!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		glClearColor(0.27f, 0.27f, 0.27f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		float speed = 0.01, time = glfwGetTime()*speed;
		double counter = ((double)((int)time%10) + time-int(time)-5)*72;
		glm::vec3 camera_pos = glm::vec3(sin(counter)*5, -0.2f, cos(counter)*5);
		glm::mat4 view = glm::lookAt(camera_pos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		glUseProgram(our_shader.program);
		glUniformMatrix4fv(glGetUniformLocation(our_shader.program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(our_shader.program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		// box render
		glBindVertexArray(VAO_box);
		glUniformMatrix4fv(glGetUniformLocation(our_shader.program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniform3f(glGetUniformLocation(our_shader.program, "cameraPos"), camera_pos.x, camera_pos.y, camera_pos.z);
		glActiveTexture(GL_TEXTURE0);
		glUniform1i(glGetUniformLocation(our_shader.program, "skybox"), 0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glUniformMatrix4fv(glGetUniformLocation(our_shader.program, "model"), 1, GL_FALSE, glm::value_ptr(model_box));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glUniformMatrix4fv(glGetUniformLocation(our_shader.program, "model"), 1, GL_FALSE, glm::value_ptr(model_box1));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		//skybox cube
		glUseProgram(skybox_shader.program);
		glUniformMatrix4fv(glGetUniformLocation(skybox_shader.program, "model"), 1, GL_FALSE, glm::value_ptr(model_skybox));
		glUniformMatrix4fv(glGetUniformLocation(skybox_shader.program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(skybox_shader.program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		glEnable(GL_CULL_FACE); // Enable face culling
		glCullFace(GL_BACK);

		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE0);
		glUniform1i(glGetUniformLocation(skybox_shader.program, "skybox"), 0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		glDisable(GL_CULL_FACE); // Disable face culling

		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &VAO_box);
	glDeleteBuffers(1, &VBO_box);

	glfwTerminate();
	return 0;
}
