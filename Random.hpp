#pragma once

#include <random>

namespace Random
{
	std::mt19937 mt(std::random_device{}());

	int get(int min, int max)
	{
		std::uniform_int_distribution<> die(min, max);
		return die(mt);
	}
}
