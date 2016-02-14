#include "Camera.h"

Camera::Camera()
{
	mPosition = glm::vec3(0.0f, -8.0f, 10.0f);

	mTarget = glm::vec3(0.0f, 0.0f, 0.0f);
	mDirection = glm::normalize(mPosition - mTarget);
	mFront = glm::vec3(0.0f, 0.0f, -1.0f);
	mUp = glm::vec3(0.0f, 1.0f, 0.0f);
	mRight = glm::normalize(glm::cross(mUp, mDirection));

	//mSpeed = 0.001f;
	mSpeed = 10.f;

	mYaw = -90.0f;
	mPitch = 0.0f;

	rotate(0.0f, 0.0f);
}

Camera::~Camera()
{
}

void Camera::forward(double deltaTime) {
	mPosition += mSpeed * mFront * (float)deltaTime;
}

void Camera::backward(double deltaTime) {
	mPosition -= mSpeed * mFront * (float)deltaTime;
}

void Camera::left(double deltaTime) {
	mPosition -= mSpeed * (float)deltaTime * glm::normalize(glm::cross(mFront, mUp)); // * (float)deltaTime)
}

void Camera::right(double deltaTime) {
	mPosition += mSpeed * (float)deltaTime * glm::normalize(glm::cross(mFront, mUp));
}

void Camera::rotate(float x, float y) {
	
	mYaw += x;
	mPitch += y;
	
	if (mPitch > 89.0f)
		mPitch = 89.0f;
	if (mPitch < -89.0f)
		mPitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(mYaw)) * cos(glm::radians(mPitch));
	front.y = sin(glm::radians(mPitch));
	front.z = sin(glm::radians(mYaw)) * cos(glm::radians(mPitch));
	mFront = glm::normalize(front);

	mUp = glm::vec3(0.0f, 1.0f, 0.0f);
	mRight = glm::normalize(glm::cross(mUp, mDirection));
}

glm::vec3 Camera::getPosition() {
	return mPosition;
}

glm::vec3 Camera::getFront() {
	return mFront;
}

glm::vec3 Camera::getUp() {
	return mUp;
}

glm::vec3 Camera::getRight() {
	return mRight;
}

float Camera::getPitch() {
	return mPitch;
}