#pragma once
#include "Model.h"

namespace nb {

class NB_API Polygon : public Model
{
public:
	Polygon();

	//设置点集合
	//side0：边0的点集合
	//side1：边1的点集合
	//ControlPointsCount：每段控制点的个数（0表示不启用贝塞尔）
	//sampleCount：每段贝塞尔的采样点数（0表示不启用贝塞尔）
	void setPoint(const std::vector<glm::vec2> &side0, const std::vector<glm::vec2> &side1, int controlPointsCount = 5, int sampleCount = 20);

private:
	std::vector<glm::vec2> doBezier(const std::vector<glm::vec2> &inputs, int controlPointsCount, int sampleCount);

};

}