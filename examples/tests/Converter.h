#pragma once
#include <vector>
#include "glm/glm.hpp"

class Converter
{
public:
	static std::vector<glm::vec2> doublesToSide(const std::vector<double> &doubles)
	{
		std::vector<glm::vec2> side(doubles.size() / 2);
		for (size_t i = 0; i != side.size(); ++i)
		{
			side[i] = { doubles[i * 2], doubles[i * 2 + 1] };
		}
		return side;
	}
};
