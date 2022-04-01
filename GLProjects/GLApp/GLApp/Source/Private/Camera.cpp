#pragma once

#include "Source/Public/Camera.h"
#include "Source/Public/EngineDebugHelper.h"
#include "Source/Public/Math.h"

//-------------------------------------------------------------------

namespace CameraStatics
{
	const GLfloat YAW = 90.0f;
	const GLfloat PITCH = 0.0f;
	const GLfloat SPEED = 6.0f;
	//Mousemovement sense
	const GLfloat SENSITIVTY = 0.15f;
	//Feild of view
	const GLfloat ZOOM = 45.0f;

	const GLfloat AspectRatio = 1920.0f / 1080.0f;
	const GLfloat NearPlane = 0.1f;
	const GLfloat FarPlane = 50.0f;
}

//-------------------------------------------------------------------

Camera::Camera(glm::vec3 InPosition, glm::vec3 InUp)
	: Front(glm::vec3(0.0f, 0.0f, -1.0f))
	, MovementSpeed(CameraStatics::SPEED)
	, MouseSensitivity(CameraStatics::SENSITIVTY)
	, Zoom(CameraStatics::ZOOM)
	, Yaw(CameraStatics::YAW)
	, Pitch(CameraStatics::PITCH)
	, AspectRatio(CameraStatics::AspectRatio)
	, FarPlane(CameraStatics::FarPlane)
	, NearPlane(CameraStatics::NearPlane)
{
	Position = InPosition;
	WorldUp = InUp;
	Yaw = Yaw;
	Pitch = Pitch;
	IsOrthagraphic = false;
	updateCameraVectors();
}

Camera::Camera(glm::vec3 InPosition, glm::vec3 InDirection, bool InIsOrthagraphic, GLfloat InAspectRatio, GLfloat InNearPlane, GLfloat InFarPlane, glm::vec3 InUp )
{
	Position = InPosition;
	WorldUp = InUp;
	Front = InDirection;
	Right = glm::normalize(glm::cross(Front, WorldUp));
	IsOrthagraphic = InIsOrthagraphic;
	AspectRatio = InAspectRatio;
	NearPlane = InNearPlane;
	FarPlane = InFarPlane;
}

//-------------------------------------------------------------------

void Camera::updateCameraVectors()
{
	glm::vec3 LocalFront;
	LocalFront.x = cos(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
	LocalFront.y = sin(glm::radians(this->Pitch));
	LocalFront.z = sin(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
	Front = glm::normalize(LocalFront);
	
	// Also re-calculate the Right and Up vector
	Right = glm::normalize(glm::cross(this->Front, this->WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look Upor down which results in slower movement.
	Up = glm::normalize(glm::cross(this->Right, this->Front));
}

//-------------------------------------------------------------------

void Camera::ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime)
{
	GLfloat velocity = (this->MovementSpeed * deltaTime) * MoMath::MoSign((float)direction);
	if (direction == Camera_Movement::EForward || direction == Camera_Movement::EBackward)
	{
		this->Position += this->Front * (velocity);
	}

	if (direction == Camera_Movement::ELeft || direction == Camera_Movement::ERight)
	{
		this->Position += this->Right * (velocity);
	}
}

//-------------------------------------------------------------------

void Camera::ProcessMouseMovement(GLfloat xOffset, GLfloat yOffset, GLboolean constrainPitch)
{
	xOffset *= MouseSensitivity;
	yOffset *= MouseSensitivity;

	Yaw += xOffset;
	Pitch += yOffset;

	if (constrainPitch)
	{
		Pitch = MoMath::MoClamp(Pitch, -89.0f, 89.0f);
	}
	updateCameraVectors();
}

//-------------------------------------------------------------------

void Camera::ProessMouseSroll(GLfloat yOffset)
{
	this->Zoom -= (yOffset / 5);
	this->Zoom = MoMath::MoClamp(this->Zoom, 1.0f, 45.0f);
}

//-------------------------------------------------------------------

GLfloat Camera::GetZoom(Camera* Target)
{
	return Target->Zoom;
}

//-------------------------------------------------------------------

glm::vec3 Camera::GetPosition(Camera* Target)
{
	return Target->Position;
}

//-------------------------------------------------------------------

glm::vec3 Camera::GetFront(Camera* Target)
{
	return Target->Front;
}

//-------------------------------------------------------------------

glm::mat4 Camera::GetProjection(Camera * Target)
{
	if (Target->IsOrthagraphic)
	{
		MoMessageLogger("Camera::GetProjection Orthagraphic GetProjection being called and isnt currently supported");
	}

	return glm::perspective(Camera::GetZoom(Target), Target->AspectRatio, Target->NearPlane, Target->FarPlane);
}

//-------------------------------------------------------------------

GLfloat Camera::GetAspectRatio(Camera * Target)
{
	return Target->AspectRatio;
}

//-------------------------------------------------------------------

GLfloat Camera::GetNearPlane(Camera * Target)
{
	return Target->NearPlane;
}

//-------------------------------------------------------------------

GLfloat Camera::GetFarPlane(Camera * Target)
{
	return Target->FarPlane;
}

//-------------------------------------------------------------------

glm::mat4 Camera::GetViewMatrix(Camera* Target)
{
	if (Target->IsOrthagraphic)
	{
		MoMessageLogger("Camera::GetViewMatrix Orthagraphic view matrix being called and isnt currently supported");
	}

	return  MoMath::MoLookAt(Target->Position, Target->Position + Target->Front, Target->Up);
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------