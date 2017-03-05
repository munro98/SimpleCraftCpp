#include "Frustum.h"
#include "BoundingBox.h"


Frustum::Frustum(const glm::mat4 &viewMatrix, const glm::mat4 &projectionMatrix)
{
	const glm::mat4 &v = viewMatrix;
	const glm::mat4 &p = projectionMatrix;

	glm::mat4 clipMatrix;

	clipMatrix[0][0] = v[0][0] * p[0][0] + v[0][1] * p[1][0] + v[0][2] * p[2][0] + v[0][3] * p[3][0];
	clipMatrix[1][0] = v[0][0] * p[0][1] + v[0][1] * p[1][1] + v[0][2] * p[2][1] + v[0][3] * p[3][1];
	clipMatrix[2][0] = v[0][0] * p[0][2] + v[0][1] * p[1][2] + v[0][2] * p[2][2] + v[0][3] * p[3][2];
	clipMatrix[3][0] = v[0][0] * p[0][3] + v[0][1] * p[1][3] + v[0][2] * p[2][3] + v[0][3] * p[3][3];
	clipMatrix[0][1] = v[1][0] * p[0][0] + v[1][1] * p[1][0] + v[1][2] * p[2][0] + v[1][3] * p[3][0];
	clipMatrix[1][1] = v[1][0] * p[0][1] + v[1][1] * p[1][1] + v[1][2] * p[2][1] + v[1][3] * p[3][1];
	clipMatrix[2][1] = v[1][0] * p[0][2] + v[1][1] * p[1][2] + v[1][2] * p[2][2] + v[1][3] * p[3][2];
	clipMatrix[3][1] = v[1][0] * p[0][3] + v[1][1] * p[1][3] + v[1][2] * p[2][3] + v[1][3] * p[3][3];
	clipMatrix[0][2] = v[2][0] * p[0][0] + v[2][1] * p[1][0] + v[2][2] * p[2][0] + v[2][3] * p[3][0];
	clipMatrix[1][2] = v[2][0] * p[0][1] + v[2][1] * p[1][1] + v[2][2] * p[2][1] + v[2][3] * p[3][1];
	clipMatrix[2][2] = v[2][0] * p[0][2] + v[2][1] * p[1][2] + v[2][2] * p[2][2] + v[2][3] * p[3][2];
	clipMatrix[3][2] = v[2][0] * p[0][3] + v[2][1] * p[1][3] + v[2][2] * p[2][3] + v[2][3] * p[3][3];
	clipMatrix[0][3] = v[3][0] * p[0][0] + v[3][1] * p[1][0] + v[3][2] * p[2][0] + v[3][3] * p[3][0];
	clipMatrix[1][3] = v[3][0] * p[0][1] + v[3][1] * p[1][1] + v[3][2] * p[2][1] + v[3][3] * p[3][1];
	clipMatrix[2][3] = v[3][0] * p[0][2] + v[3][1] * p[1][2] + v[3][2] * p[2][2] + v[3][3] * p[3][2];
	clipMatrix[3][3] = v[3][0] * p[0][3] + v[3][1] * p[1][3] + v[3][2] * p[2][3] + v[3][3] * p[3][3];

	m_planes[PLANE_RIGHT].x = clipMatrix[3][0] - clipMatrix[0][0];
	m_planes[PLANE_RIGHT].y = clipMatrix[3][1] - clipMatrix[0][1];
	m_planes[PLANE_RIGHT].z = clipMatrix[3][2] - clipMatrix[0][2];
	m_planes[PLANE_RIGHT].w = clipMatrix[3][3] - clipMatrix[0][3];

	m_planes[PLANE_LEFT].x = clipMatrix[3][0] + clipMatrix[0][0];
	m_planes[PLANE_LEFT].y = clipMatrix[3][1] + clipMatrix[0][1];
	m_planes[PLANE_LEFT].z = clipMatrix[3][2] + clipMatrix[0][2];
	m_planes[PLANE_LEFT].w = clipMatrix[3][3] + clipMatrix[0][3];

	m_planes[PLANE_BOTTOM].x = clipMatrix[3][0] + clipMatrix[1][0];
	m_planes[PLANE_BOTTOM].y = clipMatrix[3][1] + clipMatrix[1][1];
	m_planes[PLANE_BOTTOM].z = clipMatrix[3][2] + clipMatrix[1][2];
	m_planes[PLANE_BOTTOM].w = clipMatrix[3][3] + clipMatrix[1][3];

	m_planes[PLANE_TOP].x = clipMatrix[3][0] - clipMatrix[1][0];
	m_planes[PLANE_TOP].y = clipMatrix[3][1] - clipMatrix[1][1];
	m_planes[PLANE_TOP].z = clipMatrix[3][2] - clipMatrix[1][2];
	m_planes[PLANE_TOP].w = clipMatrix[3][3] - clipMatrix[1][3];

	m_planes[PLANE_BACK].x = clipMatrix[3][0] - clipMatrix[2][0];
	m_planes[PLANE_BACK].y = clipMatrix[3][1] - clipMatrix[2][1];
	m_planes[PLANE_BACK].z = clipMatrix[3][2] - clipMatrix[2][2];
	m_planes[PLANE_BACK].w = clipMatrix[3][3] - clipMatrix[2][3];

	m_planes[PLANE_FRONT].x = clipMatrix[3][0] + clipMatrix[2][0];
	m_planes[PLANE_FRONT].y = clipMatrix[3][1] + clipMatrix[2][1];
	m_planes[PLANE_FRONT].z = clipMatrix[3][2] + clipMatrix[2][2];
	m_planes[PLANE_FRONT].w = clipMatrix[3][3] + clipMatrix[2][3];

	for (int i = 0; i < 6; i++)
	{
		m_planes[i] = glm::normalize(m_planes[i]);
	}
}

// check whether an AABB intersects the frustum
int Frustum::testIntersection(const BoundingBox box) const
{
	TestResult result = Frustum::TEST_INSIDE;

	for (glm::uint i = 0; i < 6; i++)
	{
		const float pos = m_planes[i].w;
		const glm::vec3 normal = glm::vec3(m_planes[i]);

		if (glm::dot(normal, box.getPositiveVertex(normal)) + pos < 0.0f)
		{
			return Frustum::TEST_OUTSIDE;
		}

		if (glm::dot(normal, box.getNegativeVertex(normal)) + pos < 0.0f)
		{
			result = Frustum::TEST_INTERSECT;
		}
	}

	return result;
}