/*****************************************************************************
 * Author : Ram
 * Date : 4/September/2018
 * Email : ramkalath@gmail.com
 * Breif Description : specular lighting
 * Detailed Description : ambient+diffused+specular lighting. Causes a marked improvement in visual appeal.
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

struct vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 tex_coord;
};


int width = 640, height = 800;

int main()
{
	// glfw stuff ====================================================================
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow *window = glfwCreateWindow(800, 600, "specular_lighting", nullptr, nullptr);
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

	Shader our_shader("./shaders/vertex_shader.vert", "./shaders/fragment_shader.frag");
	Shader lamp_shader("./shaders/lamp_vertex_shader.vert", "./shaders/lamp_fragment_shader.frag");

	// ================================================================================
	// data - vertices
	vertex vinst;
	std::vector<vertex> v;
	int num_divisions = 100;
	for(int j=0; j<num_divisions; j++)
	{
		for(int i=0; i<num_divisions; i++)
		{
			// triangle 1 ---------------------------------------------------------------------
			vinst.position = glm::vec3((2.0f/num_divisions)*i+(-1), (2.0f/num_divisions)*j+(-1), 0); 
			vinst.normal = glm::vec3(0.0f, 0.0f, 1.0f);
			vinst.tex_coord = glm::vec2((2.0f/num_divisions)*i+(-1), (2.0f/num_divisions)*j+(-1));
			v.push_back(vinst);

			vinst.position = glm::vec3((2.0f/num_divisions)*i+(-1), (2.0f/num_divisions)*(j+1)+(-1), 0);
			vinst.normal = glm::vec3(0.0f, 0.0f, 1.0f);
			vinst.tex_coord = glm::vec2((2.0f/num_divisions)*i+(-1), (2.0f/num_divisions)*(j+1)+(-1));
			v.push_back(vinst);

			vinst.position = glm::vec3((2.0f/num_divisions)*(i+1)+(-1), (2.0f/num_divisions)*(j+1)+(-1), 0);
			vinst.normal = glm::vec3(0.0f, 0.0f, 1.0f);
			vinst.tex_coord = glm::vec2((2.0f/num_divisions)*(i+1)+(-1), (2.0f/num_divisions)*(j+1)+(-1));
			v.push_back(vinst);
			
			// triangle 2 ---------------------------------------------------------------------
			vinst.position = glm::vec3((2.0f/num_divisions)*i+(-1), (2.0f/num_divisions)*j+(-1), 0);
			vinst.normal = glm::vec3(0.0f, 0.0f, 1.0f);
			vinst.tex_coord = glm::vec2((2.0f/num_divisions)*i+(-1), (2.0f/num_divisions)*j+(-1));
			v.push_back(vinst);

			vinst.position = glm::vec3((2.0f/num_divisions)*(i+1)+(-1), (2.0f/num_divisions)*(j+1)+(-1), 0);
			vinst.normal = glm::vec3(0.0f, 0.0f, 1.0f);
			vinst.tex_coord = glm::vec2((2.0f/num_divisions)*(i+1)+(-1), (2.0f/num_divisions)*(j+1)+(-1));
			v.push_back(vinst);

			vinst.position = glm::vec3((2.0f/num_divisions)*(i+1)+(-1), (2.0f/num_divisions)*j+(-1), 0);
			vinst.normal = glm::vec3(0.0f, 0.0f, 1.0f);
			vinst.tex_coord = glm::vec2((2.0f/num_divisions)*(i+1)+(-1), (2.0f/num_divisions)*j+(-1));
			v.push_back(vinst);
		}
	}

	float *vertices = &v[0].position.x;

	//for(int i=0; i<num_divisions*num_divisions*8*6; i++) {
		//std::cout << vertices[i] << "\t";
		//if((i+1)%8==0) std::cout << std::endl;
	//}
	// ==================================================================================
	// Defining VAO and VBO
	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, num_divisions*num_divisions*6*8*sizeof(float), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (GLvoid*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float), (GLvoid*)(6*sizeof(float)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);
	// ==================================================================================
	// Lamp vertices
	
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

	// ==================================================================================
	// Defining VAO and VBO for lamp
	unsigned int VBO_lamp, VAO_lamp;
	glGenVertexArrays(1, &VAO_lamp);
	glGenBuffers(1, &VBO_lamp);
	glBindVertexArray(VAO_lamp);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_lamp);
	glBufferData(GL_ARRAY_BUFFER, sizeof(lamp_vertices), lamp_vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
	
	// ================================================================================= 
	// model view and perspective matrices
	glm::mat4 model = glm::mat4{1.0f};
	model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(2.0f, 2.0f, 0.0f));
	glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	glm::mat4 model_lamp = glm::mat4{1.0f};
	glm::vec3 lamp_pos = glm::vec3(0.0f, -0.5f, 0.8f);
	model_lamp = glm::translate(model_lamp, lamp_pos);
	model_lamp = glm::rotate(model_lamp, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 1.0f));
	model_lamp = glm::rotate(model_lamp, glm::radians(45.0f), glm::vec3(1.0f, 1.0f, 0.0f));
	model_lamp = glm::scale(model_lamp, glm::vec3(0.5f, 0.5f, 0.5f));
	glm::mat4 view_lamp = glm::lookAt(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	float angle = 45.0f;
	float n = 0.1f, f = 100.0f;
	float ar = (float)width/(float)height;
	glm::mat4 projection_perspective = {1/(ar*tan(angle/2)), 0, 0, 0, 0, 1/tan(angle/2), 0, 0, 0, 0, -(f+n)/(f-n), -2*f*n/(f-n), 0, 0, -1, 0};
	projection_perspective = glm::transpose(projection_perspective);

	// ================================================================================= 
	// ambient light
	float ambient_strength = 0.3f;
	glm::vec3 light_color = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 ambient_light = ambient_strength * light_color;

	while(!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

        glClearColor(0.27f, 0.27f, 0.27f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		// draw object ---------------------------------------------------------------
        glUseProgram(our_shader.program);

		glUniformMatrix4fv(glGetUniformLocation(our_shader.program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(glGetUniformLocation(our_shader.program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(our_shader.program, "projection"), 1, GL_FALSE, glm::value_ptr(projection_perspective));

		// ambient light
		glUniform3f(glGetUniformLocation(our_shader.program, "ambient_light"), ambient_light.x, ambient_light.y, ambient_light.z);

		// diffused light
		glUniform3f(glGetUniformLocation(our_shader.program, "lamp_pos"), lamp_pos.x, lamp_pos.y, lamp_pos.z);
		glUniform3f(glGetUniformLocation(our_shader.program, "light_color"), 1.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(our_shader.program, "box_color"), 0.7f, 0.3f, 0.3f);
		glUniform3f(glGetUniformLocation(our_shader.program, "camera_pos"), 0.0f, 0.0f, 0.0f);

		glBindVertexArray(VAO);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINES);
		glDrawArrays(GL_TRIANGLES, 0, num_divisions*num_divisions*2*3);
		glBindVertexArray(0);
		
		// draw lamp --------------------------------------------------------------------
		glUseProgram(lamp_shader.program);
		glUniformMatrix4fv(glGetUniformLocation(lamp_shader.program, "model"), 1, GL_FALSE, glm::value_ptr(model_lamp));
		glUniformMatrix4fv(glGetUniformLocation(lamp_shader.program, "view"), 1, GL_FALSE, glm::value_ptr(view_lamp));
		glUniformMatrix4fv(glGetUniformLocation(lamp_shader.program, "projection"), 1, GL_FALSE, glm::value_ptr(projection_perspective));

		glBindVertexArray(VAO_lamp);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glDeleteVertexArrays(1, &VAO_lamp);
	glDeleteBuffers(1, &VBO_lamp);
	glfwTerminate();
	return 0;
}
