#include "parkassist/gles/Polygon.h"
#include "parkassist/gles/SmartBezier.h"

using namespace nb;

void Polygon::set(const std::vector<glm::vec2>& side0, const std::vector<glm::vec2>& side1, int controlPointsCount, int sampleCount)
{
	auto s0 = side0;
	auto s1 = side1;
	if (s0.size() > s1.size())
	{
		s0.resize(s1.size());
	}
	else if (s1.size() > s0.size())
	{
		s1.resize(s0.size());
	}

	//决定是否执行贝塞尔
	bool needBezier = controlPointsCount >= 2 && sampleCount >= 2;
	if (needBezier)
	{
		s0 = doBezier(s0, controlPointsCount, sampleCount);
		s1 = doBezier(s1, controlPointsCount, sampleCount);
	}

	//顶点和顶点序列
	this->vertexs.resize(s0.size() + s1.size());
	for (size_t i = 0; i < this->vertexs.size() / 2; ++i)
	{
		this->vertexs[i * 2].position = glm::vec3(s0[i], 0.0f);
		this->vertexs[i * 2 + 1].position = glm::vec3(s1[i], 0.0f);
	}

	auto indicesCount = this->vertexs.empty() ? 0 : (this->vertexs.size() - 2) * 3;
	this->indices.resize(indicesCount);
	auto x = 0;
	for (size_t i = 0; i < this->indices.size() / 6; ++i)
	{
		this->indices[x++] = (uint16_t)(2 * i + 0);
		this->indices[x++] = (uint16_t)(2 * i + 1);
		this->indices[x++] = (uint16_t)(2 * i + 3);
		this->indices[x++] = (uint16_t)(2 * i + 0);
		this->indices[x++] = (uint16_t)(2 * i + 3);
		this->indices[x++] = (uint16_t)(2 * i + 2);
	}
}

std::vector<glm::vec2> Polygon::doBezier(const std::vector<glm::vec2>& inputs, int controlPointsCount, int sampleCount)
{
	SmartBezier sb(controlPointsCount, sampleCount);
	return sb.doBezier(inputs);
}
