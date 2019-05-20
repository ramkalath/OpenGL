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
	float width, height;

	// matrices
	glm::mat4 projection_perspective;
	glm::mat4 view;

	GameSettings();

private:
};
