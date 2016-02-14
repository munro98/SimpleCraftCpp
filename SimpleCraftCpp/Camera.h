#pragma once

#include <glm/glm.hpp>

class Camera
{
public:
	Camera::Camera();
	Camera::~Camera();

	void Camera::forward(double deltaTime);
	void Camera::backward(double deltaTime);
	void Camera::left(double deltaTime);
	void Camera::right(double deltaTime);
	void Camera::rotate(float pitch, float yaw);

	glm::vec3 Camera::getPosition();
	glm::vec3 Camera::getFront();
	glm::vec3 Camera::getUp();
	glm::vec3 Camera::getRight();

	float getPitch();


private:

	float mSpeed;
	float mPitch, mYaw;

	glm::vec3 mTarget;
	glm::vec3 mPosition;

	glm::vec3 mDirection;
	glm::vec3 mFront;
	glm::vec3 mUp;
	glm::vec3 mRight;
};