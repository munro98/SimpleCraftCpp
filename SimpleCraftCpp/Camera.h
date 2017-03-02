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

	float m_speed;
	float m_pitch, m_yaw;

	glm::vec3 m_target;
	glm::vec3 m_position;

	glm::vec3 m_direction;
	glm::vec3 m_front;
	glm::vec3 m_up;
	glm::vec3 m_right;
};