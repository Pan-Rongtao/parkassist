#include "parkassist/gles/PolygonGeometry.h"
#include "bezier.h"
#include "parkassist/gles/SmartBezier.h"

using namespace nb;

PolygonGeometry::PolygonGeometry()
{
}

void PolygonGeometry::setPoint(const std::vector<glm::vec2>& side0, const std::vector<glm::vec2>& side1, int controlPointsCount, int sampleCount)
{
	auto s0 = side0;
	auto s1 = side1;

	if (s0.size() > s1.size())		s0.resize(s1.size());
	else if (s1.size() > s0.size())	s1.resize(s0.size());

	//决定是否执行贝塞尔
	if (controlPointsCount >= 2 && sampleCount >= 2)
	{
		s0 = doBezier(s0, controlPointsCount, sampleCount);
		s1 = doBezier(s1, controlPointsCount, sampleCount);
		if (s0.size() != s1.size())
		{
			nbThrowException(std::runtime_error, "do doBezier s0.size != s1.size");
		}
	}

	std::vector<Vertex> vertexs(s0.size() + s1.size());
	for (size_t i = 0; i < vertexs.size() / 2; ++i)
	{
		vertexs[i * 2].position.x = s0[i].x;
		vertexs[i * 2].position.y = s0[i].y;
		vertexs[i * 2].position.z = 0;
		vertexs[i * 2 + 1].position.x = s1[i].x;
		vertexs[i * 2 + 1].position.y = s1[i].y;
		vertexs[i * 2 + 1].position.z = 0;
	}

	auto indicesCount = vertexs.empty() ? 0 :(vertexs.size() - 2) * 3;
	std::vector<uint16_t> indices(indicesCount);
	auto x = 0;
	for (size_t i = 0; i < indices.size() / 6; ++i)
	{
		indices[x++] = (unsigned short)(2 * i + 0);
		indices[x++] = (unsigned short)(2 * i + 1);
		indices[x++] = (unsigned short)(2 * i + 3);
		indices[x++] = (unsigned short)(2 * i + 0);
		indices[x++] = (unsigned short)(2 * i + 3);
		indices[x++] = (unsigned short)(2 * i + 2);
	}
	this->vertexs = vertexs;
	this->indices = indices;
}

glm::vec4 PolygonGeometry::getBox()
{
	return glm::vec4();
}

std::vector<glm::vec2> PolygonGeometry::doBezier(const std::vector<glm::vec2>& inputs, int controlPointsCount, int sampleCount)
{
	SmartBezier sb(controlPointsCount, sampleCount);
	auto x = sb.doBezier(inputs);
	return x;
}
