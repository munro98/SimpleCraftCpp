#include "RayCast.h"

RayCast::RayCast()
{
}

RayCast::~RayCast()
{
}

bool RayCast::clipLine(int d, float x, float y, float z, const glm::vec3& v0, const glm::vec3& v1, float& f_low, float& f_high)
{

	float f_dim_low, f_dim_high;


	if (d == 0)
	{
		f_dim_low = (x -.5f - v0.x) / (v1.x - v0.x);
		f_dim_high = (x + .5f - v0.x) / (v1.x - v0.x);
	}
	else if (d == 1)
	{
		f_dim_low = (y -.5f - v0.y) / (v1.y - v0.y);
		f_dim_high = (y + .5f - v0.y) / (v1.y - v0.y);
	}
	else
	{
		f_dim_low = (z -.5f - v0.z) / (v1.z - v0.z);
		f_dim_high = (z + .5f - v0.z) / (v1.z - v0.z);
	}

	if (f_dim_high < f_dim_low)
		swap(f_dim_high, f_dim_low);

	if (f_dim_high < f_low)
		return false;

	if (f_dim_low > f_high)
		return false;

	f_low = max(f_dim_low, f_low);
	f_high = min(f_dim_high, f_high);

	return true;
}

bool RayCast::lineBlockIntersection(float x, float y, float z, const glm::vec3& v0, const glm::vec3&& v1, glm::vec3& vecIntersection, float& flFraction)
{
	float f_low = 0;
	float f_high = 1;

	if (!clipLine(0, x, y, z, v0, v1, f_low, f_high))
		return false;

	if (!clipLine(1, x, y, z, v0, v1, f_low, f_high))
		return false;

	if (!clipLine(2, x, y, z, v0, v1, f_low, f_high))
		return false;

	glm::vec3 delta = v1 - v0;
	vecIntersection = v0 + delta * f_low;

	flFraction = min(flFraction, f_low);

	return true;
}