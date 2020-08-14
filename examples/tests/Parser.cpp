#include "Parser.h"
#include <fstream>
#include "spdlog/spdlog.h"
#include "glm/glm.hpp"
#include "parkassist/gles/Program.h"
#include "parkassist/gles/Renderer.h"
#include "parkassist/gles/PolygonGeometry.h"
#include "GLES2/gl2.h"

using namespace nb;
using namespace nlohmann;

constexpr const char *CfgFile = "config.json";
constexpr const char *StateFileHead = "state";

void Parser::setDir(const std::string & dir)
{
	m_dir = dir + "/";
	spdlog::info("set dir [{}]", m_dir);
}

std::string Parser::getDir() const
{
	return m_dir;
}

bool Parser::parse()
{
	try {
		auto k = getTickCount();
		parseConfig();
		parseStates();
		spdlog::info("{} cost [{}] ms", __FUNCTION__, getTickCount() - k);
		return true;
	}
	catch (std::exception &ex)
	{
		spdlog::error("{}", ex.what());
		return false;
	}
}

int Parser::getContextWidth() const
{
	return m_contextWidth;
}

int Parser::getContextHeight() const
{
	return m_contextHeight;
}

int Parser::drawingStatesCount() const
{
	return m_drawingStates.size();
}

std::vector<PolygonPtr> Parser::getDrawingState(int index) const
{
	if (index < 0 || index >= (int)m_drawingStates.size())
	{
		nbThrowException(std::out_of_range, "index[%d] is out of range[0, %zu)", index, m_drawingStates.size());
	}
	return m_drawingStates[index];
}

void Parser::parseConfig()
{
	spdlog::info("parsing [{}] ...", CfgFile);
	json obj = parseOneFile(CfgFile);

	auto check = [&obj](const std::string &nodeName, int minuxLimit)->int
	{
		int ret = 0;
		json j;
		try {
			j = obj.at(nodeName);
		}
		catch (...)
		{
			throw Exception(fmt::format("can't find field [{}] as int type in file [{}]", nodeName, CfgFile));
		}

		if (!j.is_number_integer())
		{
			throw Exception(fmt::format("can't find field [{}] as int type in file [{}]", nodeName, CfgFile));
		}
		else
		{
			ret = j;
			if (ret < minuxLimit)
			{
				throw Exception(fmt::format("[{}] must >= 1 but not [{}]", nodeName, ret));
			}
		}
		return ret;
	};

	m_contextWidth = check("ContextWidth", 0);
	m_contextHeight = check("ContextHeight", 0);
	m_stateCount = check("StateCount", 0);
	m_bezierControlPointsCount = check("BezierControlPointsCount", 0);
	m_bezierSampleCount = check("BezierSampleCount", 0);

	spdlog::info("parsing [{}] success, ContextWidth=[{}], ContextHeight=[{}], StateCount=[{}], BezierControlPointsCount=[{}], BezierSampleCount=[{}]", 
		CfgFile, m_contextWidth, m_contextHeight, m_stateCount, m_bezierControlPointsCount, m_bezierSampleCount);
}

void Parser::parseStates()
{
	m_drawingStates.resize(m_stateCount);
	for (size_t i = 0; i != m_drawingStates.size(); ++i)
	{
		m_parsingFileName = fmt::format("{}{}.json", StateFileHead, i);
		spdlog::info("parsing [{}] ...", m_parsingFileName);
		json obj = parseOneFile(m_parsingFileName);
		auto state = makeDrawingState(obj);
		m_drawingStates[i] = state;
	}
}

json Parser::parseOneFile(const std::string & fileName)
{
	std::ifstream stream(m_dir + fileName.data());
	if (!stream)
	{
		throw FileNotExistsException(m_dir + fileName);
	}

	json obj;
	try {
		stream >> obj;
	}
	catch (std::exception &e) 
	{
		stream.close();
		throw JsonParsingException(m_dir + fileName, e.what());
	}
	return obj;
}

std::vector<PolygonPtr> Parser::makeDrawingState(const json &obj)
{
	std::vector<PolygonPtr> state;
	for (auto iter = obj.begin(); iter != obj.end(); ++iter)
	{
		auto item = iter.value();
		if (!item.is_object())
		{
			throw Exception(fmt::format("[{}] is not a [Polygon Type] in file [{}], please remove it", iter.key(), m_parsingFileName));
		}

		//这里的if其实没有本质意义，因为如果不是true，已经抛出异常了。因此这里不需要else
		if (isPolygon(item, iter.key()))
		{
			std::vector<float> s0 = item["Side0"];
			std::vector<float> s1 = item["Side1"];
			if (s0.size() > s1.size())	s0.resize(s1.size());
			else if(s1.size() > s0.size())	s1.resize(s0.size());
			std::vector<glm::vec2> side0(s0.size() / 2);
			std::vector<glm::vec2> side1(s0.size() / 2);
			for (size_t i = 0; i != side0.size(); ++i)
			{
				side0[i] = { s0[i * 2], s0[i * 2 + 1] };
				side1[i] = { s1[i * 2], s1[i * 2 + 1] };
			}
			auto polygon = std::make_shared<Polygon>(side0, side1);
			polygon->setBezierParams(m_bezierControlPointsCount, m_bezierSampleCount);
			auto brush = toBrush(item["Color"]);
			polygon->setBrush(brush);

			state.push_back(polygon);
		}
	}
	return state;
}

bool Parser::isPoints(const json & arr)
{
	if (arr.size() % 2 != 0)
	{
		throw InvalidArraySizeException();
	}

	bool bIsIntegerArray = false;
	try {
		bIsIntegerArray = isIntegerArray(arr);
	}
	catch (InvalidArrayValueException &ex)
	{
		throw ex;
	}
	return bIsIntegerArray;
}

BrushPtr Parser::toBrush(const json & arr)
{
	if (arr.empty() || (arr.size() % 4 != 0 && arr.size() % 5 != 0))
	{
		throw InvalidArraySizeException();
	}

	BrushPtr brush;
	if (arr.size() == 4)
	{
		Color color{ (uint8_t)arr[0], (uint8_t)arr[1], (uint8_t)arr[2], (uint8_t)arr[3] };
		brush = std::make_shared<SolidColorBrush>(color);
	}
	else
	{
		std::vector<GradientStop> gradientStops;
		for (size_t k = 0; k < arr.size() / 5; ++k)
		{
			Color color((uint8_t)arr[k * 5 + 0], (uint8_t)arr[k * 5 + 1], (uint8_t)arr[k * 5 + 2], (uint8_t)arr[k * 5 + 3]);
			float offset = (float)arr[k * 5 + 4];
			auto stop = GradientStop{ color, offset };
			gradientStops.push_back(stop);
		}
		brush = std::make_shared<LinearGradientBrush>(gradientStops);
	}

	return brush;
}

bool Parser::isPolygon(const json & obj, const std::string &polygonName)
{
	json side0, side1, color;
	try { side0 = obj.at("Side0"); }catch(...) { throw Exception(fmt::format("[{}.Side0] not found in file [{}]", polygonName, m_parsingFileName)); }
	try { side1 = obj.at("Side1"); }catch(...) { throw Exception(fmt::format("[{}.Side1] not found in file [{}]", polygonName, m_parsingFileName)); }
	try { color = obj.at("Color"); }catch(...) { throw Exception(fmt::format("[{}.Color] not found in file [{}]", polygonName, m_parsingFileName)); }
	bool sizeEqual = side0.size() == side1.size();

	bool ret = false;
	try { ret = isPoints(side0); }
	catch (InvalidArrayValueException &e)	{ throw Exception(fmt::format("[{}.Side0] must be a int array as [Points Type], invalid value index=[{}]", polygonName, e.InvalidIndex)); }
	catch (InvalidArraySizeException &e)	{ (void)e; throw Exception(fmt::format("[{}.Side0].size must be a multiple of 2", polygonName)); }

	try { ret &= isPoints(side1); }
	catch (InvalidArrayValueException &e)	{ throw Exception(fmt::format("[{}.Side1] must be a int array as [Points Type], invalid value index=[{}]", polygonName, e.InvalidIndex)); }
	catch (InvalidArraySizeException &e)	{ (void)e; throw Exception(fmt::format("[{}.Side1].size must be a multiple of 2", polygonName)); }

	try { ret &= toBrush(color) != nullptr; }
	catch (InvalidArrayValueException &e)	{ throw Exception(fmt::format("[{}.Color] must be a int array as [Color Type], invalid value index=[{}]", polygonName, e.InvalidIndex)); }
	catch (InvalidArraySizeException &e)	{ (void)e; throw Exception(fmt::format("[{}.Color].size must be 4 as [Color Type]", polygonName)); }

	if (!sizeEqual)
	{
		int x = 10;
		++x;
	//	throw Exception(fmt::format("[{}.Side0.size={}], [{}.Side1.size={}] are not same in file [{}]", polygonName, side0.size(), polygonName, side1.size(), m_parsingFileName));
	}
	return ret;
}

bool Parser::isIntegerArray(const json & arr)
{
	for (size_t i = 0; i != arr.size(); ++i)
	{
		if (!arr[i].is_number())
			throw InvalidArrayValueException(i);
	}
	return true;
}
