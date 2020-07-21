#pragma once
#include "parkassist/gles/Def.h"
#include "glm/glm.hpp"

namespace nb {

class Brush;
using BrushPtr = std::shared_ptr<Brush>;
class NB_API Polygon
{
public:
	//构建一个多边形，它两侧的顶点为side0和side1（一般而言，顶点数不可太少也不可能太多，太少则不够顺滑，太多则影响贝塞尔策略）
	Polygon();
	Polygon(const std::vector<glm::vec2> &side0, const std::vector<glm::vec2> &side1);

	//设置顶点
	void setSide0(const std::vector<glm::vec2> &side0);
	void setSide1(const std::vector<glm::vec2> &side1);

	//贝塞尔参数
	//controlPointsCount：每段控制点数，默认值为5
	//sampleCount：每段贝塞尔采集点数，默认值20
	void setBezierParams(int controlPointsCount, int sampleCount);

	//设置画刷
	void setBrush(BrushPtr brush);

	//
	void setMode(uint8_t mode);

	void draw();

private:
	std::vector<glm::vec2> m_side0;
	std::vector<glm::vec2> m_side1;
	int m_controlPointsCount;
	int m_sampleCount;
	BrushPtr m_brush;
	uint8_t m_drawMode;
};

}