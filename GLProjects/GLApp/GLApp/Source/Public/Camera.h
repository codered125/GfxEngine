#pragma once
#define GLEW_STATIC
#include <vector>
#include <GL/glew.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include "Math.h"

enum Camera_Movement
{
	EForward = 1, 
	EBackward = -1,
	ELeft = -2,
	ERight = 2
};

const GLfloat YAW = -90.0f;
const GLfloat PITCH = 0.0f;
const GLfloat SPEED = 6.0f;

//Mousemovement sense
const GLfloat SENSITIVTY = 0.15f;

//Feild of view
const GLfloat ZOOM = 45.0f; 

class Camera 
{
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

	void updateCameraVectors()
	{
		glm::vec3 front;
		front.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
		front.y = sin(glm::radians(this->pitch));
		front.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
		this->front = glm::normalize(front);
		// Also re-calculate the Right and Up vector
		this->right = glm::normalize(glm::cross(this->front, this->worldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		this->up = glm::normalize(glm::cross(this->right, this->front));
	}

public:
	// Constructor with vectors
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), GLfloat yaw = YAW, GLfloat pitch = PITCH) : front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVTY), zoom(ZOOM)
	{
		this->position = position;
		this->worldUp = up;
		this->yaw = yaw;
		this->pitch = pitch;
		this->updateCameraVectors();
	}

	// Constructor with scalar values
	Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch) : front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVTY), zoom(ZOOM)
	{
		this->position = glm::vec3(posX, posY, posZ);
		this->worldUp = glm::vec3(upX, upY, upZ);
		this->yaw = yaw;
		this->pitch = pitch;
		this->updateCameraVectors();
	}

	glm::mat4 GetViewMatrix()
	{	
		return glm::lookAt(this->position, this->position + this->front, this->up); 
	}

	void ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime)
	{
		GLfloat velocity = (this->movementSpeed * deltaTime) * MoMath::MoSign(direction);
		
		(direction == Camera_Movement::EForward || direction == Camera_Movement::EBackward) ?
			this->position += this->front * (velocity) : this->position += this->right * (velocity);

		/*
		if (direction == EForward) this->position  += this->front * velocity;
		if (direction == EBackward)	this->position -=  this->front * velocity;
		if (direction == ELeft)	this->position -= this->right * velocity;
		if (direction == ERight)this->position += this->right * velocity;
		*/
	}
	
	void ProcessMouseMovement(GLfloat xOffset, GLfloat yOffset, GLboolean constrainPitch = true) 
	{
		xOffset *= this->mouseSensitivity;
		yOffset *= this->mouseSensitivity;

		this->yaw  += xOffset;
		this->pitch += yOffset;

		if (constrainPitch)
		{
			if (this->pitch > 89.0f)this->pitch = 89.0f;
			if (this->pitch < -89.0f)this->pitch = -89.0f;
		}
		this->updateCameraVectors();
	}
	
	void ProessMouseSroll(GLfloat yOffset)
	{
		if (this->zoom >= 1.0f && this->zoom <= 45.0f) this->zoom -= (yOffset / 5);
		if (this->zoom <= 1.0f) this->zoom = 1.0f;
		if (this->zoom >= 45.0f) this->zoom = 45.0f;
	}

	GLfloat GetZoom() 
	{ 
		return this->zoom; 
	}

	glm::vec3 getPosition()
	{
		return this->position;
	}

	glm::vec3 getFront()
	{
		return this->front;
	}
};