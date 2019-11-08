/*****************************************************************************
* Filename : main.cpp
* Date : 30-9-2019
* Author : Ram
* Email : ramkalath@gmail.com
* Breif Description : Shadows
* Detailed Description : 1) DONE: Render floor with proper texturing.
*								DONE: yet to create the second object
* 						 2) TODO(ram): lighting is all screwed up. Create a proper library for lighting
								TODO(ram): test if each component of lighting works well.
* 						 3) TODO(ram): first pass for shadowing.
* 						 4) TODO(ram): second pass for shadowing.
*****************************************************************************/

// GLEW and GLFW includes
// Use this for ycm autocompletion
#include "use_glew.h"
#ifdef USE_GLEW
	#define GLEW_STATIC
    #include <GL/glew.h>
#else
    #define GL_GLEXT_PROTOTYPES
    #include <GL/gl.h>
    #include <GL/glext.h>
#endif
#include <GLFW/glfw3.h>
#include <SOIL/SOIL.h>

// glm includes
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other popular includes
#include <vector>
#include <iostream>
#include <math.h>

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

std::vector<float> populate_vertices(int no_of_tiles_width, int no_of_tiles_height) // segment floor width; segment floor height
{

	std::vector<float> v;

	float sfh = 1/(float)no_of_tiles_height;
	float sfw = 1/(float)no_of_tiles_width;

	for(double h=-0.5; h<0.5;){ 
		for(double w=-0.5; w<0.5;){
			//------------ positions ----------------------- | ---------------- normals ---------------------------- | ----- tex coords ---------------- |
			v.push_back(w); v.push_back(h); v.push_back(0.0f); v.push_back(0.0f); v.push_back(0.0f); v.push_back(1.0f); v.push_back(w); v.push_back(h);
			v.push_back(w+sfw); v.push_back(h); v.push_back(0.0f); v.push_back(0.0f); v.push_back(0.0f); v.push_back(1.0f); v.push_back(w+sfw); v.push_back(h);
			v.push_back(w+sfw); v.push_back(h+sfh); v.push_back(0.0f); v.push_back(0.0f); v.push_back(0.0f); v.push_back(1.0f); v.push_back(w+sfw); v.push_back(h+sfh);
			v.push_back(w); v.push_back(h); v.push_back(0.0f); v.push_back(0.0f); v.push_back(0.0f); v.push_back(1.0f); v.push_back(w); v.push_back(h);
			v.push_back(w+sfw); v.push_back(h+sfh); v.push_back(0.0f); v.push_back(0.0f); v.push_back(0.0f); v.push_back(1.0f); v.push_back(w+sfw); v.push_back(h+sfh);
			v.push_back(w); v.push_back(h+sfh); v.push_back(0.0f); v.push_back(0.0f); v.push_back(0.0f); v.push_back(1.0f); v.push_back(w); v.push_back(h+sfh);
			w=w+sfw;
		}
		h=h+sfh;
	}
	return v;
}

int main()
{
	// global settings
	glm::vec3 camera_pos = glm::vec3(0.0f, 0.0f, 5.0f);
	GameSettings gs(1200, 800, camera_pos);

	// glfw stuff ====================================================================
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	GLFWwindow *window = glfwCreateWindow(gs.width, gs.height, "specular_lighting", nullptr, nullptr);
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

	glfwGetFramebufferSize(window, &gs.width, &gs.height);
	glViewport(0, 0, gs.width, gs.height);

	Shader objectshader("./shaders/vertex_shader.vert", "./shaders/fragment_shader.frag");

	// ---------------------------------------------------------------------------------------------------------
	// defining floor vertices
	int no_of_tiles_width = 10, no_of_tiles_height = 10;
	std::vector<float> floor = populate_vertices(no_of_tiles_width, no_of_tiles_height);
	float *floor_vertices = &floor[0];
	int no_floor_floats = 6 * no_of_tiles_width * no_of_tiles_height * 8; // TODO(ram): what does that 6 do?

	unsigned int VBO_floor, VAO_floor;
	glGenVertexArrays(1, &VAO_floor);
	glGenBuffers(1, &VBO_floor);
	glBindVertexArray(VAO_floor);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_floor);
	glBufferData(GL_ARRAY_BUFFER, no_floor_floats*sizeof(float), floor_vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (GLvoid*)0); // positions
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat))); // normals
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (GLvoid*)(6*sizeof(GLfloat))); // textures
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);

	glm::mat4 floor_model_matrix = glm::mat4{1.0f};
	floor_model_matrix = glm::translate(floor_model_matrix, glm::vec3(0.0f, -0.3f, 0.0f));
	floor_model_matrix = glm::rotate(floor_model_matrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	floor_model_matrix = glm::scale(floor_model_matrix, glm::vec3(3.0f, 3.0f, 0.0f));

	// ---------------------------------------------------------------------------------------------------------
	// defining plate vertices
	int no_plate_width = 5, no_plate_height = 5;
	std::vector<float> plate = populate_vertices(no_plate_width, no_plate_height);
	float *plate_vertices = &plate[0];
	int no_plate_floats = 6 * no_plate_width * no_plate_height * 8; // TODO(ram): what does that 6 do?

	unsigned int VBO_plate, VAO_plate;
	glGenVertexArrays(1, &VAO_plate);
	glGenBuffers(1, &VBO_plate);
	glBindVertexArray(VAO_plate);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_plate);
	glBufferData(GL_ARRAY_BUFFER, no_plate_floats*sizeof(float), plate_vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (GLvoid*)0); // positions
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat))); // normals
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (GLvoid*)(6*sizeof(GLfloat))); // textures
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);

	glm::mat4 plate_model_matrix = glm::mat4{1.0f};
	plate_model_matrix = glm::translate(plate_model_matrix, glm::vec3(0.0f, 0.0f, -2.0f));
	//plate_model_matrix = glm::rotate(plate_model_matrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	//plate_model_matrix = glm::scale(plate_model_matrix, glm::vec3(0.5f, 0.5f, 0.0f));

	// ---------------------------------------------------------------------------------------------------------
	/* Textures */
	unsigned int floor_texture;
	glGenTextures(1, &floor_texture);
	glBindTexture(GL_TEXTURE_2D, floor_texture);
	
	int width_texture, height_texture;
	unsigned char* image1 = SOIL_load_image("./resources/floor_texture.png", &width_texture, &height_texture, 0, SOIL_LOAD_RGB);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width_texture, height_texture, 0, GL_RGB, GL_UNSIGNED_BYTE, image1);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image1);
	glBindTexture(GL_TEXTURE_2D, 0);

	unsigned int plate_texture;
	glGenTextures(1, &plate_texture);
	glBindTexture(GL_TEXTURE_2D, plate_texture);

	unsigned char* image2 = SOIL_load_image("./resources/pebbles.png", &width_texture, &height_texture, 0, SOIL_LOAD_RGB);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width_texture, height_texture, 0, GL_RGB, GL_UNSIGNED_BYTE, image2);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image2);
	glBindTexture(GL_TEXTURE_2D, 0);
	// ---------------------------------------------------------------------------------------------------------

	//glm::vec3 light_vector = normalize(glm::vec3(10.0f, 10.0f, 0.0f));
	////std::cout << light_vector.x << "\t" << light_vector.y << "\t" << light_vector.z << std::endl;
	//glm::vec3 Norm = glm::vec3(0.0f, 1.0f, 0.0f);
	////Norm = normalize(glm::vec3(transpose(inverse(floor_model_matrix)) * glm::vec4(Norm, 1.0f)));
	//std::cout << Norm.x << "\t" << Norm.y << "\t" << Norm.z << std::endl;
	//float diff = glm::max(dot(Norm, light_vector), 0.0f);
	//std::cout << diff << std::endl;

	std::cout << floor_model_matrix[0][0] << "\t" << floor_model_matrix[0][1] << "\t" << floor_model_matrix[0][2] << "\t" << plate_model_matrix[0][3] << std::endl;
	std::cout << floor_model_matrix[1][0] << "\t" << floor_model_matrix[1][1] << "\t" << floor_model_matrix[1][2] << "\t" << plate_model_matrix[1][3] << std::endl;
	std::cout << floor_model_matrix[2][0] << "\t" << floor_model_matrix[2][1] << "\t" << floor_model_matrix[2][2] << "\t" << plate_model_matrix[2][3] << std::endl;
	std::cout << floor_model_matrix[3][0] << "\t" << floor_model_matrix[3][1] << "\t" << floor_model_matrix[3][2] << "\t" << plate_model_matrix[3][3] << std::endl;
	std::cout << std::endl;
	std::cout << plate_model_matrix[0][0] << "\t" << plate_model_matrix[0][1] << "\t" << plate_model_matrix[0][2] << "\t" << plate_model_matrix[0][3] << std::endl;
	std::cout << plate_model_matrix[1][0] << "\t" << plate_model_matrix[1][1] << "\t" << plate_model_matrix[1][2] << "\t" << plate_model_matrix[1][3] << std::endl;
	std::cout << plate_model_matrix[2][0] << "\t" << plate_model_matrix[2][1] << "\t" << plate_model_matrix[2][2] << "\t" << plate_model_matrix[2][3] << std::endl;
	std::cout << plate_model_matrix[3][0] << "\t" << plate_model_matrix[3][1] << "\t" << plate_model_matrix[3][2] << "\t" << plate_model_matrix[3][3] << std::endl;
	std::cout << std::endl;

	while(!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		glClearColor(0.27f, 0.27f, 0.27f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(objectshader.program);

		// ---------------------------------------------------------------------------------------------------------
		/* Floor object */
		glBindVertexArray(VAO_floor);
		// Activate floor texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, floor_texture);
		// Upload uniforms
		glUniform1i(glGetUniformLocation(objectshader.program, "my_texture"), 0);
		glUniformMatrix4fv(glGetUniformLocation(objectshader.program, "model"), 1, GL_FALSE, glm::value_ptr(floor_model_matrix));
		glUniformMatrix4fv(glGetUniformLocation(objectshader.program, "view"), 1, GL_FALSE, glm::value_ptr(gs.view));
		glUniformMatrix4fv(glGetUniformLocation(objectshader.program, "projection"), 1, GL_FALSE, glm::value_ptr(gs.projection_perspective));

		glUniform3f(glGetUniformLocation(objectshader.program, "materialambient"), 1.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(objectshader.program, "materialdiffuse"), 0.5f, 0.5f, 0.5f);
		glUniform3f(glGetUniformLocation(objectshader.program, "materialspecular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(objectshader.program, "materialspecular"), 16.0f);

		glUniform3f(glGetUniformLocation(objectshader.program, "LightDirection"), 0.0f, 3.0f, 3.0f);
		glUniform3f(glGetUniformLocation(objectshader.program, "LightAmbient"), 0.5f, 0.5f, 0.5f);
		glUniform3f(glGetUniformLocation(objectshader.program, "LightDiffuse"), 0.1f, 0.1f, 0.1f);
		glUniform3f(glGetUniformLocation(objectshader.program, "LightSpecular"), 0.2f, 0.2f, 0.2f);
		glUniform3f(glGetUniformLocation(objectshader.program, "CameraPosition"), camera_pos.x, camera_pos.y, camera_pos.z);
		// Draw
		//glPolygonMode( GL_FRONT_AND_BACK, GL_LINE ); // Draw wireframe
		glDrawArrays(GL_TRIANGLES, 0, no_floor_floats); // draw call
		// ---------------------------------------------------------------------------------------------------------
		/* plate object */
		glBindVertexArray(VAO_plate);
		// Activate plate texture
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, plate_texture);
		// Upload uniforms
		glUniform1i(glGetUniformLocation(objectshader.program, "my_texture"), 1);
		glUniformMatrix4fv(glGetUniformLocation(objectshader.program, "model"), 1, GL_FALSE, glm::value_ptr(plate_model_matrix));
		// Draw
		//glPolygonMode( GL_FRONT_AND_BACK, GL_LINE ); // Draw wireframe
		glDrawArrays(GL_TRIANGLES, 0, no_plate_floats); // draw call
		// ---------------------------------------------------------------------------------------------------------

		glBindVertexArray(0);
		glfwSwapBuffers(window);
	}
	glfwTerminate();
	return 0;
}
