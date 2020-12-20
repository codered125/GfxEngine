#pragma once
#include <vector>
#include <GL/glew.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

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
	Camera(glm::vec3 InPosition = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 InUp = glm::vec3(0.0f, 1.0f, 0.0f));

	// Constructor with exposed values for fake views
	Camera(glm::vec3 InPosition, glm::vec3 InDirection, bool InIsOrthagraphic, GLfloat InAspectRatio, GLfloat InNearPlane, GLfloat InFarPlane, glm::vec3 InUp = glm::vec3(0.0f, 1.0f, 0.0f));

	// Constructor with scalar values
	//Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch);

	void ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime);

	void ProcessMouseMovement(GLfloat xOffset, GLfloat yOffset, GLboolean constrainPitch = true);

	void ProessMouseSroll(GLfloat yOffset);

	static GLfloat GetZoom(Camera* Target);

	static glm::vec3 getPosition(Camera* Target);

	static glm::vec3 getFront(Camera* Target);

	static glm::mat4 GetViewMatrix(Camera* Target);

	static glm::mat4 GetProjection(Camera* Target);

	static GLfloat GetAspectRatio(Camera* Target);

	static GLfloat GetNearPlane(Camera* Target);

	static GLfloat GetFarPlane(Camera* Target);



private:

	void updateCameraVectors();

	// Camera Attributes
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;

	// Eular Angles
	GLfloat Yaw;
	GLfloat Pitch;

	// Camera options
	GLfloat MovementSpeed;
	GLfloat MouseSensitivity;
	GLfloat Zoom;

	GLfloat AspectRatio;
	GLfloat NearPlane;
	GLfloat FarPlane;

	bool IsOrthagraphic;

};

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------