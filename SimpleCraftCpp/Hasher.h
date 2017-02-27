#pragma once
#include <cstdlib>

namespace Hasher
{
	size_t hash(int integer);
	size_t hash(int x, int y);
	size_t hash(int x, int y, int z);
}