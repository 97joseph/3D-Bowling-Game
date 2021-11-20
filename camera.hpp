#pragma once

#include "Dependencies\glm\glm.hpp"
#include "Dependencies\glm\gtc\matrix_transform.hpp"
#include "Dependencies\glm\gtc\type_ptr.hpp"

class Camera
{
private:
	glm::vec3 cameraPosition;
	glm::vec3 cameraViewDirection;
	glm::vec3 cameraUp;
	glm::vec3 right;

public:
	Camera();
	Camera(glm::vec3 cameraPosition);
	Camera(glm::vec3 cameraPosition, glm::vec3 cameraViewDirection, glm::vec3 cameraUp);
	~Camera();

	glm::vec3 getCameraPosition();
	glm::vec3 getCameraViewDirection();
	glm::vec3 getCameraUp();

	void setCameraPosition(glm::vec3 cameraPosition);
	void setCameraViewDirection(glm::vec3 cameraViewDirection);
	void setCameraUp(glm::vec3 cameraUp);

	void rotateOx(float angle);
	void rotateOy(float angle);

	void translateFront(float speed);
	void translateBack(float speed);
	void translateUp(float speed);
	void translateDown(float speed);
};