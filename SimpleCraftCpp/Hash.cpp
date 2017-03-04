#include "Hash.h"
#include <memory>

namespace Hash
{
	int hash(int key)
	{
		key = ~key + (key << 15);
		key = key ^ (key >> 12);
		key = key + (key << 2);
		key = key ^ (key >> 4);
		key = key * 2048;
		key = key ^ (key >> 16);
		return std::hash<int>{}(key);
	}

	int hash(int x, int y)
	{
		x = hash(x);
		y = hash(y);
		return hash(x ^ (y << 2));
	}

	long long int pack(int x, int y)
	{
		long long int key = (long long int) x << 32 | y;
		return std::hash<long long int>{}(key);
	}

	int unpack(long long int key)
	{
		int x = (int)((key & 0xFFFFFFFF00000000LL) >> 32);
		int y = (int)(key & 0xFFFFFFFF00000000LL);
		return std::hash<long long int>{}(key);
	}

}
