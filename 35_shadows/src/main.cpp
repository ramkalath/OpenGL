/*****************************************************************************
 * Author : Ram
 * Date : 2/08/2019
 * Email : ramkalath@gmail.com
 * Breif Description : shadows
 * Detailed Description : yet to complete the basic setup for creating shadows; yet to draw a few boxes
 *****************************************************************************/
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include <SOIL/SOIL.h>
#include "../include/shader.h"
#include "../include/modelloader.h"
#include "../include/gamesettings.h"

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
	glEnable(GL_MULTISAMPLE);

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
	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, num_divisions*num_divisions*6*8*sizeof(float), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (GLvoid*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (GLvoid*)(3*sizeof(float)));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float), (GLvoid*)(6*sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);

	unsigned int texture_floor;
	int width_texture, height_texture;
	glGenTextures(1, &texture_floor);
	glBindTexture(GL_TEXTURE_2D, texture_floor);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	unsigned char* image = SOIL_load_image("./resources/floor_texture.jpg", &width_texture, &height_texture, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width_texture, height_texture, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	// ==================================================================================
	// Lamp vertices
	// TODO(ram): convert these vertices into vertices for a cube with normals and tex coords
	// use the same for defining the lamp too
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
	
	// ================================================================================= 
	// model view and perspective matrices
	glm::mat4 model_floor = glm::mat4(5.0f, 0.0f, 0.0f, 0.0f,
									  0.0f, 5.0f, 0.0f, 0.0f,
									  0.0f, 0.0f, 1.0f, 0.0f,
									  0.0f, 0.0f, 0.0f, 1.0f);

	float rot_angle_x = glm::radians(90.0f);
	glm::mat4 rotation_x = glm::mat4(1.0f, 0.0f,        0.0f,       0.0f,
									 0.0f, cos(rot_angle_x), -sin(rot_angle_x), 0.0f,
									 0.0f, sin(rot_angle_x),  cos(rot_angle_x), 0.0f,
									 0.0f, 0.0f,        0.0f,       1.0f);

	model_floor =  rotation_x * model_floor;

	glm::vec3 lamp_pos = glm::vec3(0.0f, 2.8f, -2.0f);
	glm::mat4 model_lamp = glm::mat4(0.2f, 0.0f, 0.0f, 0.0f,
									 0.0f, 0.2f, 0.0f, 0.0f,
									 0.0f, 0.0f, 0.2f, 0.0f,
									 lamp_pos.x, lamp_pos.y, lamp_pos.z, 1.0f);

	// View matrices declared now but will be defined later
	glm::mat4 view_floor = glm::mat4(1.0f);
	glm::mat4 view_lamp = glm::mat4(1.0f);

	// defining perspective projection
	float angle = 45.0f, n = 0.1f, f = 100.0f, ar = (float)width/(float)height;
	glm::mat4 projection_perspective = {1/(ar*tan(angle/2)), 0, 0, 0, 
										0, 1/tan(angle/2), 0, 0, 
										0, 0, -(f+n)/(f-n), -2*f*n/(f-n), 
										0, 0, -1, 0};
	projection_perspective = glm::transpose(projection_perspective);

	// ================================================================================= 
	// ambient light
	float ambient_strength = 0.3f;
	glm::vec3 light_color = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 ambient_light = ambient_strength * light_color;

	while(!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		float theta = glm::radians((fmod(glfwGetTime(), 10.0)/10)*360);
		//theta = glm::radians(0.4f);
		float distance = 6.0;
		glm::vec3 view_pos = glm::vec3(distance*cos(theta), 4.0f, distance*sin(theta));
		view_floor = glm::lookAt(view_pos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		view_lamp = glm::lookAt(view_pos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		
		// draw object ---------------------------------------------------------------
        glUseProgram(our_shader.program);

		glUniformMatrix4fv(glGetUniformLocation(our_shader.program, "model"), 1, GL_FALSE, glm::value_ptr(model_floor));
		glUniformMatrix4fv(glGetUniformLocation(our_shader.program, "view"), 1, GL_FALSE, glm::value_ptr(view_floor));
		glUniformMatrix4fv(glGetUniformLocation(our_shader.program, "projection"), 1, GL_FALSE, glm::value_ptr(projection_perspective));

		// ambient light
		glUniform3f(glGetUniformLocation(our_shader.program, "ambient_light"), ambient_light.x, ambient_light.y, ambient_light.z);

		// diffused light
		glUniform3f(glGetUniformLocation(our_shader.program, "lamp_pos"), lamp_pos.x, lamp_pos.y, lamp_pos.z);
		glUniform3f(glGetUniformLocation(our_shader.program, "light_color"), 1.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(our_shader.program, "floor_color"), 0.1f, 0.5f, 0.5f);
		glUniform3f(glGetUniformLocation(our_shader.program, "camera_pos"), view_pos.x, view_pos.y, view_pos.z);

		glBindVertexArray(VAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture_floor);
		glUniform1i(glGetUniformLocation(our_shader.program, "mytexture"), 0);
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
