#pragma once
#include <algorithm>
#include <glm/detail/type_vec3.hpp>
#include "Block.h"

class Block;
using std::swap;
using std::min;
using std::max;

class RayCast
{
public:
	RayCast();
	~RayCast();

	static bool clipLine(int d, float x, float y, float z, const glm::vec3 & v0, const glm::vec3 & v1, float & f_low, float & f_high);
	static bool lineBlockIntersection(float x, float y, float z, const glm::vec3& v0, const glm::vec3&& v1, glm::vec3& vecIntersection, float& flFraction);
};

