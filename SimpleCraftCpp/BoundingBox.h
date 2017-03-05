#pragma once
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class BoundingBox
{
public:
	BoundingBox(glm::vec3 minimum, glm::vec3 maximum, glm::vec3 position);

	glm::vec3 getPositiveVertex(const glm::vec3 &normal) const;
	glm::vec3 getNegativeVertex(const glm::vec3 &normal) const;

	glm::vec3 minimum;
	glm::vec3 maximum;
	glm::vec3 position;

	//TestResult testIntersection(const glm::vec3 &point) const;
	//TestResult testIntersection(std::shared_ptr<const BoundingBox> box) const;
	//TestResult testIntersection(std::shared_ptr<const BoundingSphere> sphere) const;
};
