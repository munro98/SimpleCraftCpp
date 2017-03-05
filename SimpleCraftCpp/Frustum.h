#pragma once
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "BoundingBox.h"

class Frustum
{
public:

	enum Plane
	{
		PLANE_BACK,
		PLANE_FRONT,
		PLANE_RIGHT,
		PLANE_LEFT,
		PLANE_TOP,
		PLANE_BOTTOM
	};

	enum TestResult
	{
		TEST_OUTSIDE,
		TEST_INTERSECT,
		TEST_INSIDE
	};

public:

	Frustum(const glm::mat4 &viewMatrix, const glm::mat4 &projectionMatrix);

	const glm::vec4 &getPlane(const int plane) const;

	int testIntersection(const glm::vec3 &point) const;
	int testIntersection(const BoundingBox box) const;
	//TestResult testIntersection(std::shared_ptr<const BoundingSphere> sphere) const;

protected:
	glm::vec4 m_planes[6];
};

