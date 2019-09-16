/*****************************************************************************
 * Author : Ram
 * Date : 31/Aug/2019
 * Email : ramkalath@gmail.com
 * Breif Description : shadows
 * Detailed Description : Create a template for shadows. Create a common lighting scheme.
 *****************************************************************************/

// Use this for ycm autocompletion
#include "use_glew.h" // insert #define USE_GLEW in use_glew.h
#ifdef USE_GLEW
	#define GLEW_STATIC
    #include <GL/glew.h>
#else
    #define GL_GLEXT_PROTOTYPES
    #include <GL/gl.h>
    #include <GL/glext.h>
#endif
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

int WIDTH = 640, HEIGHT = 800;

int main()
{
	GameSettings globalsettings;
	// glfw stuff ====================================================================
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Lighting Properties", nullptr, nullptr);
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

	glfwGetFramebufferSize(window, &WIDTH, &HEIGHT);
	glViewport(0, 0, WIDTH, HEIGHT);

	// ==================================================================================================
	// Lamp
	GLfloat lamp_vertices[] = 
	{
		-0.5f, -0.5f, -0.5f,  0.5f, -0.5f, -0.5f,  0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f, -0.5f,  0.5f, -0.5f, -0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,  0.5f, -0.5f,  0.5f,  0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f, -0.5f,  0.5f,  0.5f, -0.5f, -0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f, -0.5f,  0.5f, -0.5f, -0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f, -0.5f, -0.5f,  0.5f, -0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,  0.5f,  0.5f, -0.5f,  0.5f, -0.5f, -0.5f, 
		 0.5f, -0.5f, -0.5f,  0.5f, -0.5f,  0.5f,  0.5f,  0.5f,  0.5f,
		-0.5f, -0.5f, -0.5f,  0.5f, -0.5f, -0.5f,  0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f, -0.5f, -0.5f,  0.5f, -0.5f, -0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,  0.5f,  0.5f, -0.5f,  0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f, -0.5f,  0.5f,  0.5f, -0.5f,  0.5f, -0.5f
	};
	unsigned int VBO_lamp, VAO_lamp;
	glGenVertexArrays(1, &VAO_lamp);
	glGenBuffers(1, &VBO_lamp);
	glBindVertexArray(VAO_lamp);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_lamp);
	glBufferData(GL_ARRAY_BUFFER, sizeof(lamp_vertices), lamp_vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	glm::vec3 lamp_pos = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::mat4 model_lamp = glm::mat4(0.2f, 0.0f, 0.0f, 0.0f,
									 0.0f, 0.2f, 0.0f, 0.0f,
									 0.0f, 0.0f, 0.2f, 0.0f,
									 lamp_pos.x, lamp_pos.y, lamp_pos.z, 1.0f);

	// ==================================================================================================

	Shader objectshader("./shaders/vertex_shader.vert", "./shaders/fragment_shader.frag");
	Shader lampshader("./shaders/lamp_vertex_shader.vert", "./shaders/lamp_fragment_shader.frag");
	Shader depthshader("./shaders/vertex_depthshader.vert", "./shaders/fragment_depthshader.frag");

	// load box and some presets
	Modelloader box("./resources/multisample_cube/multisample_cube.obj");
	box.modelmatrix = { 1.0f, 0.0f, 0.0f, 0.0f,
						0.0f, 1.0f, 0.0f, 0.0f, 
						0.0f, 0.0f, 1.0f, 0.0f, 
						0.0f, 1.0f, 0.0f, 1.0f};
	box.modelmatrix = glm::translate(box.modelmatrix, glm::vec3(0.0f, -1.0f, 0.0f));
	box.modelmatrix = glm::scale(box.modelmatrix, glm::vec3(0.3f, 0.3f, 0.3f));

	Modelloader floor("./resources/floor/floor8.obj");
	floor.modelmatrix = {1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f};
	floor.modelmatrix = glm::translate(floor.modelmatrix, glm::vec3(0.0f, -2.0f, 0.0f));
	floor.modelmatrix = glm::rotate(floor.modelmatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	floor.modelmatrix = glm::scale(floor.modelmatrix, glm::vec3(5.0f, 5.0f, 5.0f));

	// --------------------------------------------------------------------------------------------------
	// generating a framebuffer
	unsigned int depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);

	// generating a depth map texture
	unsigned int depthMap;
	unsigned int SHADOW_WIDTH, SHADOW_HEIGHT;
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Attaching depthMap to the framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE); // framebuffer needs a color buffer; these statements override that
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	// --------------------------------------------------------------------------------------------------

	glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 10.0f);
	//TODO(ram): we have to fix this to ensure the shadow gets projected at the right place
	// find out where the camera is
	// Also find out where the center of the object is
	glm::mat4 lightView = glm::lookAt(lamp_pos, glm::vec3(0.0f), glm::vec3(1.0f));
	glm::mat4 lightSpaceMatrix = lightProjection * lightView;
	

	while(!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glViewport(0, 0, 1024, 1024);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);


		glViewport(0, 0, WIDTH, HEIGHT); 
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Light space transform
		float near_plane = 1.0f, far_plane = 10.0f;
		ConfigureShaderAndMatrices();
		glBindTexture(GL_TEXTURE_2D, depthMap);
		RenderScene();

		// draw object ---------------------------------------------------------------
		glUseProgram(objectshader.program);

		 // Uniforms
		glUniformMatrix4fv(glGetUniformLocation(objectshader.program, "view"), 1, GL_FALSE, glm::value_ptr(globalsettings.view));
		glUniformMatrix4fv(glGetUniformLocation(objectshader.program, "projection"), 1, GL_FALSE, glm::value_ptr(globalsettings.projection_perspective));
		glUniform3f(glGetUniformLocation(objectshader.program, "lamp_pos"), lamp_pos.x, lamp_pos.y, lamp_pos.z);
		glUniform3f(glGetUniformLocation(objectshader.program, "LightAmbient"), 0.5f, 0.5f, 0.5f);
		glUniform3f(glGetUniformLocation(objectshader.program, "LightDiffuse"), 1.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(objectshader.program, "LightSpecular"), 1.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(objectshader.program, "CameraPosition"), 0.0f, 0.0f, 0.0f);

		// render box
		glUniformMatrix4fv(glGetUniformLocation(objectshader.program, "model"), 1, GL_FALSE, glm::value_ptr(box.modelmatrix));
		box.Draw(objectshader);

		// render floor
		glUniformMatrix4fv(glGetUniformLocation(objectshader.program, "model"), 1, GL_FALSE, glm::value_ptr(floor.modelmatrix));
		floor.Draw(objectshader);

		// render lamp
		glUseProgram(lampshader.program);
		glUniformMatrix4fv(glGetUniformLocation(objectshader.program, "model"), 1, GL_FALSE, glm::value_ptr(model_lamp));
		glUniformMatrix4fv(glGetUniformLocation(objectshader.program, "view"), 1, GL_FALSE, glm::value_ptr(globalsettings.view));
		glUniformMatrix4fv(glGetUniformLocation(objectshader.program, "projection"), 1, GL_FALSE, glm::value_ptr(globalsettings.projection_perspective));
		glBindVertexArray(VAO_lamp);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
	}

	glfwTerminate();
    return 0;
}
