#include "Camera.h"

Camera::Camera()
{
	m_position = glm::vec3(0.0f, 8.0f, 10.0f);

	m_target = glm::vec3(0.0f, 0.0f, 0.0f);
	m_direction = glm::normalize(m_position - m_target);
	m_front = glm::vec3(0.0f, 0.0f, -1.0f);
	m_up = glm::vec3(0.0f, 1.0f, 0.0f);
	m_right = glm::normalize(glm::cross(m_up, m_direction));

	//mSpeed = 0.001f;
	m_speed = 10.f;

	m_yaw = -90.0f;
	m_pitch = 0.0f;

	rotate(0.0f, 0.0f);
}

Camera::~Camera()
{
}

void Camera::forward(double deltaTime) {
	m_position += m_speed * m_front * (float)deltaTime;
}

void Camera::backward(double deltaTime) {
	m_position -= m_speed * m_front * (float)deltaTime;
}

void Camera::left(double deltaTime) {
	m_position -= m_speed * (float)deltaTime * glm::normalize(glm::cross(m_front, m_up)); // * (float)deltaTime)
}

void Camera::right(double deltaTime) {
	m_position += m_speed * (float)deltaTime * glm::normalize(glm::cross(m_front, m_up));
}

void Camera::rotate(float x, float y) {
	
	m_yaw += x;
	m_pitch += y;
	
	if (m_pitch > 89.0f)
		m_pitch = 89.0f;
	if (m_pitch < -89.0f)
		m_pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	front.y = sin(glm::radians(m_pitch));
	front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	m_front = glm::normalize(front);

	m_up = glm::vec3(0.0f, 1.0f, 0.0f);
	m_right = glm::normalize(glm::cross(m_up, m_direction));
}

glm::vec3 Camera::getPosition() {
	return m_position;
}

glm::vec3 Camera::getFront() {
	return m_front;
}

glm::vec3 Camera::getUp() {
	return m_up;
}

glm::vec3 Camera::getRight() {
	return m_right;
}

float Camera::getPitch() {
	return m_pitch;
}