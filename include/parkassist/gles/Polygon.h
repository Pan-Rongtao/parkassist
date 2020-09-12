#pragma once
#include "parkassist/gles/Mesh.h"
#include "glm/glm.hpp"

namespace nb {

class NB_API Polygon : public Mesh
{
public:
	//设置（一般而言，顶点数不可太少也不可能太多，太少则不够顺滑，太多则影响贝塞尔策略）
	void set(const std::vector<glm::vec2> &side0, const std::vector<glm::vec2> &side1, int controlPointsCount = 10, int sampleCount = 50);

private:
	std::vector<glm::vec2> doBezier(const std::vector<glm::vec2> &inputs, int controlPointsCount, int sampleCount);

};

using PolygonPtr = std::shared_ptr<Polygon>;
}