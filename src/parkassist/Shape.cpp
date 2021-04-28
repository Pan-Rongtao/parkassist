#include "parkassist/Shape.h"
#include "clipper.hpp"
#include "earcut.hpp"
#include <array>

using namespace nb;
using namespace ClipperLib;

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
#include "parkassist/SmartBezier.h"

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

//椭圆的点计算公式为：x = a * cos(α); y = b * sin(α)
//顶点越多越圆滑

Eclipse::Eclipse()
{
}

void Eclipse::set(const glm::vec2 & center, float r, int vertexCount)
{
	set(center, r, r, vertexCount);
}

void Eclipse::set(const glm::vec2 & center, float a, float b, int vertexCount)
{
	auto getIndices = [vertexCount]()->std::vector<uint16_t>
	{
		auto count = 3 * (vertexCount - 1);
		std::vector<uint16_t> indices(count);
		for (int i = 0; i != vertexCount - 2; ++i)
		{
			int base = 3 * i;
			indices[base] = 0;
			indices[base + 1] = i + 1;
			indices[base + 2] = i + 2;
		}
		indices[count - 3] = 0;
		indices[count - 2] = vertexCount - 1;
		indices[count - 1] = 1;
		return indices;
	};

	vertexs.resize(vertexCount);
	//中心点
	vertexs[0].position = { center.x, center.y, 0.0 };
	vertexs[0].texCoord = glm::vec2(0.5, 0.5);
	auto radianStep = 6.2831853071796 / (vertexCount - 2);
	for (int i = 1; i != vertexCount; ++i)
	{
		auto radian = radianStep * i;
		vertexs[i].position = glm::vec3(center.x + a * cos(radian), center.y + b * sin(radian), 0.0);
		vertexs[i].texCoord = glm::vec2(0.5 * cos(radian) + 0.5, 1.0 - (0.5 * sin(radian) + 0.5));
	}
	indices = getIndices();
}

void Polyline::set(const std::vector<glm::vec2>& points, float thickness, int controlPointsCount, int sampleCount)
{
	auto s0 = points;
	//决定是否执行贝塞尔
	//bool needBezier = controlPointsCount >= 2 && sampleCount >= 2;
	//if (needBezier)
	//{
	//	SmartBezier sb(controlPointsCount, sampleCount);
	//	s0 = sb.doBezier(points);
	//}

	Path path;
	for (auto const &p : s0)
	{
		path.push_back(IntPoint((cInt)(p.x), (cInt)(p.y)));
	}
	ClipperOffset co;
	co.AddPath(path, jtRound, etOpenButt);
	Paths solution;
	co.Execute(solution, thickness / 2.0);

	std::vector<glm::vec2> pointsX(solution[0].size());
	for (auto i = 0u; i < pointsX.size(); ++i)
	{
		pointsX[i] = { solution[0][i].X, solution[0][i].Y };
	}

	vertexs.resize(pointsX.size());
	for (auto i = 0u; i < vertexs.size(); ++i)
	{
		auto p0 = pointsX[i] + 0.0f;
		vertexs[i].position = glm::vec3(p0, 0.0f);
	}

	using Point = std::array<float, 2>;
	std::vector<std::vector<Point>> polygon;
	std::vector<Point> pointsxx;
	for (auto i = 0u; i < vertexs.size(); ++i)
	{
		Point p = { vertexs[i].position.x, vertexs[i].position.y };
		pointsxx.push_back(p);
	}
	polygon.push_back(pointsxx);
	indices = mapbox::earcut<uint16_t>(polygon);
}
