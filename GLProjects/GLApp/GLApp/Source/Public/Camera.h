#pragma once
#include "Source/Public/Math.h"

//-------------------------------------------------------------------

enum Camera_Movement
{
	EForward = 1,
	EBackward = -1,
	ELeft = -2,
	ERight = 2
};

//-------------------------------------------------------------------

class Camera
{
public:
	// Constructor with vectors
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f));

	// Constructor with scalar values
	Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch);

	glm::mat4 GetViewMatrix();

	void ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime);

	void ProcessMouseMovement(GLfloat xOffset, GLfloat yOffset, GLboolean constrainPitch = true);

	void ProessMouseSroll(GLfloat yOffset);

	GLfloat GetZoom();

	glm::vec3 getPosition();

	glm::vec3 getFront();

private:
	// Camera Attributes
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	// Eular Angles
	GLfloat yaw;
	GLfloat pitch;

	// Camera options
	GLfloat movementSpeed;
	GLfloat mouseSensitivity;
	GLfloat zoom;

	void updateCameraVectors();
};

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------