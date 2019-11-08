#pragma once
// glm includes
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other popular includes
#include <iostream>
#include <math.h>

class GameSettings
{
public:
	// window settings
	int width, height;

	// matrices
	glm::mat4 projection_perspective;
	glm::mat4 view;

	GameSettings(int, int, glm::vec3);

private:
};

GameSettings::GameSettings(int w, int h, glm::vec3 camera_pos)
{
	// window settings
	this->width = w;
	this->height = h;

	// view matrix
	this->view = glm::lookAt(camera_pos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	
	// perspective projection matrix
	float angle = 45.0f;
	float n = 0.1f, f = 100.0f;
	float ar = (float)width/(float)height;
	this->projection_perspective = {1/(ar*tan(angle/2)), 0, 0, 0, 0, 1/tan(angle/2), 0, 0, 0, 0, -(f+n)/(f-n), -2*f*n/(f-n), 0, 0, -1, 0};
	this->projection_perspective = glm::transpose(this->projection_perspective);

	//this->projection_perspective = glm::perspective(45.0f, (float)this->width/(float)this->height, 0.1f, 100.0f);
}
