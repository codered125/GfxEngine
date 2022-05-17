#pragma once

#include <GL/glew.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <vector>

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
	Camera(const float InAspectRatio, glm::vec3 InPosition = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 InUp = glm::vec3(0.0f, 1.0f, 0.0f));

	// Constructor with exposed values for fake views
	Camera(glm::vec3 InPosition, glm::vec3 InDirection, bool InIsOrthagraphic, float InAspectRatio, float InNearPlane, float InFarPlane, glm::vec3 InUp = glm::vec3(0.0f, 1.0f, 0.0f));

	void ProcessKeyboard(Camera_Movement direction, float deltaTime);
	void ProcessMouseMovement(float xOffset, float yOffset, GLboolean constrainPitch = true);
	void ProessMouseSroll(float yOffset);

	static float GetZoom(Camera* Target);
	static glm::vec3 GetPosition(Camera* Target);
	static glm::vec3 GetFront(Camera* Target);
	static glm::mat4 GetViewMatrix(Camera* Target);
	static glm::mat4 GetProjection(Camera* Target);
	static glm::mat4 GetProjection(Camera* Target, const float InNearPlane, const float InFarPlane);
	static float GetAspectRatio(Camera* Target);
	static float GetNearPlane(Camera* Target);
	static float GetFarPlane(Camera* Target);



private:

	void updateCameraVectors();

	// Camera Attributes
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;

	// Eular Angles
	float Yaw;
	float Pitch;

	// Camera options
	float MovementSpeed;
	float MouseSensitivity;
	float Zoom;

	float AspectRatio;
	float NearPlane;
	float FarPlane;

	bool IsOrthagraphic;

};

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------