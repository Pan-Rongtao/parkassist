#include "parkassist/gles/Polygon.h"
#include "bezier.h"

using namespace nb;

void Polygon::setPoint(const std::vector<glm::vec2>& side0, const std::vector<glm::vec2>& side1)
{
	if (side0.size() != side1.size())
	{
		nbThrowException(std::logic_error, "side0.size is not same with side1.size");
	}

	//执行贝塞尔
	auto bzPts0 = doBezier(side0);
	auto bzPts1 = doBezier(side1);
	if (side0.size() != side1.size())
	{
		nbThrowException(std::runtime_error, "do doBezier bzPts0.size != bzPts1.size");
	}

	std::vector<Vertex> vertexs(bzPts0.size() + bzPts1.size());
	for (size_t i = 0; i < vertexs.size() / 2; ++i)
	{
		vertexs[i * 2].position.x = bzPts0[i].x;
		vertexs[i * 2].position.y = bzPts0[i].y;
		vertexs[i * 2].position.z = 0;
		vertexs[i * 2 + 1].position.x = bzPts1[i].x;
		vertexs[i * 2 + 1].position.y = bzPts1[i].y;
		vertexs[i * 2 + 1].position.z = 0;
	}

	auto indicesCount = (vertexs.size() - 2) * 3;
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
	meshes.push_back(Mesh(vertexs, indices));
}

std::vector<glm::vec2> Polygon::doBezier(const std::vector<glm::vec2>& inputs)
{
	auto bezier2 = [](const std::vector<Bezier::Point> &bzPts)->std::vector<glm::vec2>
	{
		std::vector<glm::vec2> ret;
		Bezier::Bezier<2> bz(bzPts);
		for (float i = 0.0f; i <= 1.000001; i += 0.1f)
		{
			Bezier::Point p = bz.valueAt(i);
			ret.push_back({ p.x, p.y });
		}
		return ret;
	};

	auto bezier3 = [](const std::vector<Bezier::Point> &bzPts)->std::vector<glm::vec2>
	{
		std::vector<glm::vec2> ret;
		Bezier::Bezier<3> bz(bzPts);
		for (float i = 0.0f; i <= 1.000001; i += 0.1f)
		{
			Bezier::Point p = bz.valueAt(i);
			ret.push_back({ p.x, p.y });
		}
		return ret;
	};

	//如果小于3个点，不需要执行贝塞尔（因为二次方贝塞尔最少需要三个点）
	if (inputs.size() < 3)
	{
		return inputs;
	}

	std::vector<glm::vec2> ret;
	for (size_t i = 0; i + 2 <= inputs.size();)
	{
		std::vector<glm::vec2> segPts;
		//如果剩余四个点，则启用三次方；剩余三个点则启用二次方（但如果启用三次方后，只剩余两个点，则应该在前一个段中启用二次方，以保证最后一段能够启用二次方贝塞尔）
		//如果还剩余四个点，表示有启用三次方的条件，内部继续判断
			//使用四个点后仍然剩余三个点来启用二次方，则正式判定为可启用三次方
		if (i + 4 == inputs.size() || i + 4 + 2 <= inputs.size())
		{
			std::vector<Bezier::Point> bzPts{ { inputs[i].x, inputs[i].y },{ inputs[i + 1].x, inputs[i + 1].y }, { inputs[i + 2].x, inputs[i + 2].y },{ inputs[i + 3].x, inputs[i + 3].y } };
			segPts = bezier3(bzPts);
			i += 3; //移动i到三次方的末位点，该店即为下一个次方的起始点
		}
		//否则执行二次方
		else
		{
			std::vector<Bezier::Point> bzPts{ { inputs[i].x, inputs[i].y },{ inputs[i + 1].x, inputs[i + 1].y }, { inputs[i + 2].x, inputs[i + 2].y }};
			segPts = bezier2(bzPts);
			i += 2;
		}
		ret.insert(ret.end(), segPts.begin(), segPts.end());
	}
	return ret;
}
