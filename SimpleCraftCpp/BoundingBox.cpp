#include "BoundingBox.h"

BoundingBox::BoundingBox(glm::vec3 minimum, glm::vec3 maximum, glm::vec3 position = glm::vec3(0.0f))
{
	this->minimum = minimum;
	this->maximum = maximum;

	this->position = position;
}

glm::vec3 BoundingBox::getPositiveVertex(const glm::vec3 &normal) const
{
	glm::vec3 positiveVertex = minimum;

	if (normal.x >= 0.0f) positiveVertex.x = maximum.x;
	if (normal.y >= 0.0f) positiveVertex.y = maximum.y;
	if (normal.z >= 0.0f) positiveVertex.z = maximum.z;

	return position + positiveVertex;
}

glm::vec3 BoundingBox::getNegativeVertex(const glm::vec3 &normal) const
{
	glm::vec3 negativeVertex = maximum;

	if (normal.x >= 0.0f) negativeVertex.x = minimum.x;
	if (normal.y >= 0.0f) negativeVertex.y = minimum.y;
	if (normal.z >= 0.0f) negativeVertex.z = minimum.z;

	return position + negativeVertex;
}