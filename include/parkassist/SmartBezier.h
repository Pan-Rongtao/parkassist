#pragma once
#include "Def.h"
#include "glm/glm.hpp"
#include <vector>

namespace nb {

class NB_API SmartBezier
{
public:
	SmartBezier(int maxControlPointsCount, int sampleCount);

	//maxSteps：最大阶
	//sampleCountOfStep：采样数
	std::vector<glm::vec2> doBezier(const std::vector<glm::vec2> &points);

private:
	int	m_maxControlPointsCount;
	int	m_sampleCount;
};

}