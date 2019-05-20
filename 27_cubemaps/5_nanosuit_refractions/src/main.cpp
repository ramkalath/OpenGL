/*****************************************************************************
 * Author : Ram
 * Date : 8/Apr/2019
 * Email : ramkalath@gmail.com
 * Breif Description : nanosuit - refractions
 * Detailed Description : nanosuit with refractions; waterman
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
#include <SOIL.h>
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
	Shader skybox_shader("./shaders/skybox_vertex_shader.vert", "./shaders/skybox_fragment_shader.frag");
	modelloader nanosuit("./resources/nanosuit/nanosuit.obj");
	nanosuit.modelmatrix = {1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f};
	nanosuit.modelmatrix = glm::translate(nanosuit.modelmatrix, glm::vec3(0.0f, -2.0f, 0.0f));
	nanosuit.modelmatrix = glm::scale(nanosuit.modelmatrix, glm::vec3(0.2f, 0.2f, 0.2f));
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
	unsigned char *image;
	int width_texture, height_texture;
	glGenTextures(1, &cubemapTexture);
	glBindTexture(GL_TEXTURE_2D, cubemapTexture);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	for(unsigned int i=0; i<faces.size(); i++) {
		image = SOIL_load_image(faces[i], &width_texture, &height_texture, 0, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i, 0, GL_RGB, width_texture, height_texture, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	}
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	
	// skybox scale
	float skybox_scale = 50.0f;
	glm::mat4 model_skybox = glm::mat4(1.0f);
	model_skybox = glm::scale(model_skybox, glm::vec3(skybox_scale, skybox_scale, skybox_scale));

	while(!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		float speed = 0.01, time = glfwGetTime()*speed;
		double counter = ((double)((int)time%10) + time-int(time)-5)*72;
		glm::vec3 camera_pos = glm::vec3(sin(counter)*5, -0.2f, cos(counter)*5);
		glm::mat4 view = glm::lookAt(camera_pos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		// draw nanosuit ---------------------------------------------------------------
        glUseProgram(objectshader.program);
		glUniformMatrix4fv(glGetUniformLocation(objectshader.program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(objectshader.program, "projection"), 1, GL_FALSE, glm::value_ptr(globalsettings.projection_perspective));
		glUniformMatrix4fv(glGetUniformLocation(objectshader.program, "model"), 1, GL_FALSE, glm::value_ptr(nanosuit.modelmatrix));
		glUniform3f(glGetUniformLocation(objectshader.program, "cameraPos"), camera_pos.x, camera_pos.y, camera_pos.z);
		glActiveTexture(GL_TEXTURE0);
		glUniform1i(glGetUniformLocation(objectshader.program, "skybox"), 0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		nanosuit.RenderModel();

		//skybox cube ---------------------------------------------------------------
		glUseProgram(skybox_shader.program);
		glUniformMatrix4fv(glGetUniformLocation(skybox_shader.program, "model"), 1, GL_FALSE, glm::value_ptr(model_skybox));
		glUniformMatrix4fv(glGetUniformLocation(skybox_shader.program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(skybox_shader.program, "projection"), 1, GL_FALSE, glm::value_ptr(globalsettings.projection_perspective));
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

	glfwTerminate();
    return 0;
}
