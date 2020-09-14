#pragma once
#include "parkassist/gles/Mesh.h"

namespace nb {

class NB_API Rectangle : public Mesh
{
public:
	Rectangle();

	void set(const glm::vec2 &p0, const glm::vec2 &p1, const glm::vec2 &p2, const glm::vec2 &p3);
};

class NB_API Polygon : public Mesh
{
public:
	//设置（一般而言，顶点数不可太少也不可能太多，太少则不够顺滑，太多则影响贝塞尔策略）
	void set(const std::vector<glm::vec2> &side0, const std::vector<glm::vec2> &side1, int controlPointsCount = 10, int sampleCount = 50);

private:
	std::vector<glm::vec2> doBezier(const std::vector<glm::vec2> &inputs, int controlPointsCount, int sampleCount);

};

using RectanglePtr = std::shared_ptr<Rectangle>;
using PolygonPtr = std::shared_ptr<Polygon>;
}