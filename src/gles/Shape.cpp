#include "parkassist/gles/Shape.h"

using namespace nb;

Rectangle::Rectangle()
{
	vertexs.resize(4);
	vertexs[0].texCoord = glm::vec2(0.0, 1.0);
	vertexs[1].texCoord = glm::vec2(1.0, 1.0);
	vertexs[2].texCoord = glm::vec2(1.0, 0.0);
	vertexs[3].texCoord = glm::vec2(0.0, 0.0);
	indices = {0, 1, 2, 0, 2, 3};
}

void Rectangle::set(const glm::vec2 & p0, const glm::vec2 & p1, const glm::vec2 & p2, const glm::vec2 & p3)
{
	vertexs[0].position = glm::vec3(p0, 0.0f);
	vertexs[1].position = glm::vec3(p1, 0.0f);
	vertexs[2].position = glm::vec3(p2, 0.0f);
	vertexs[3].position = glm::vec3(p3, 0.0f);
}

/////////////////////
#include "parkassist/gles/SmartBezier.h"

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
