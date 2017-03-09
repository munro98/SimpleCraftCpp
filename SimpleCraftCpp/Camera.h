#pragma once

#include <glm/glm.hpp>

class Camera
{
public:
	Camera();
	~Camera();

	void forward(double deltaTime);
	void backward(double deltaTime);
	void left(double deltaTime);
	void right(double deltaTime);
	void rotate(float pitch, float yaw);

	void setPosition(glm::vec3 & position);

	glm::vec3& getPosition();
	glm::vec3& getFront();
	glm::vec3& getUp();
	glm::vec3& getRight();

	float getPitch();
	float getYaw();

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