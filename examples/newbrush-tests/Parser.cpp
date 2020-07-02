#include "Parser.h"
#include <fstream>
#include "spdlog/spdlog.h"
#include "glm/glm.hpp"
#include "Exceptions.h"
#include "newbrush/gles/Program.h"
#include "newbrush/gles/Quadrangle.h"
#include "newbrush/gles/RenderObject.h"
#include "newbrush/gles/Polygon.h"

using namespace nb;
using namespace nlohmann;

constexpr char *CfgFile = "config.json";
constexpr char *StateFileHead = "state";

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
		auto k = nb::getTickCount();
		parseConfig();
		parseStates();
		spdlog::info("{} cost [{}] ms", __FUNCTION__, nb::getTickCount() - k);
		return true;
	}
	catch (std::exception &ex)
	{
		spdlog::error("{}", ex.what());
		return false;
	}
}

const std::vector<DrawingState>& Parser::drawingStates() const
{
	return m_drawingStates;
}

void Parser::parseConfig()
{
	spdlog::info("parsing [{}] ...", CfgFile);
	json obj = parseOneFile(CfgFile);
	json jStateCount;
	try {
		jStateCount = obj.at("StateCount");
	}
	catch (...)
	{
		throw Exception(fmt::format("can't find field [StateCount] as int type in file [{}]", CfgFile));
	}

	if (!jStateCount.is_number_integer())
	{
		throw Exception(fmt::format("can't find field [StateCount] as int type in file [{}]", CfgFile));
	}
	else
	{
		m_stateCount = jStateCount;
		if (m_stateCount < 1)
		{
			throw Exception(fmt::format("[StateCount] must >= 1 but not [{}]", m_stateCount));
		}
	}

	spdlog::info("parsing [{}] success, StateCount=[{}]", CfgFile, m_stateCount);
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
		throw FileNotExistsException(fileName);
	}

	json obj;
	try {
		stream >> obj;
	}
	catch (std::exception &e) 
	{
		stream.close();
		throw JsonParsingException(fileName, e.what());
	}
	return obj;
}

DrawingState Parser::makeDrawingState(const json &obj)
{
	DrawingState state;
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
			std::vector<int> arr = item["SolidColor"];
			std::vector<glm::vec2> side0(s0.size() / 2);
			std::vector<glm::vec2> side1(s0.size() / 2);
			glm::vec4 solidColor{ arr[0] / 255.0f, arr[1] / 255.0f, arr[2] / 255.0f, arr[3] / 255.0f };
			for (size_t i = 0; i != side0.size(); ++i)
			{
				side0[i] = { s0[i * 2], s0[i * 2 + 1] };
				side1[i] = { s1[i * 2], s1[i * 2 + 1] };
			}
			auto polygon = std::make_shared<nb::Polygon>();
			polygon->setPoint(side0, side1);
			auto renderObj = std::make_shared<RenderObject>(polygon, Programs::primitive());
			renderObj->model()->unifyColor(solidColor);
			state.push_back(renderObj);
		}
	}
	return state;
}

ValueType Parser::testType(const json & j)
{
	if (j.is_boolean())				return ValueType::boolean;
	else if (j.is_number_integer())	return ValueType::integer;
	else if (j.is_number_float())	return ValueType::real;
	else if (j.is_string())			return ValueType::string;
	else if (j.is_object())
	{
		return testObject(j);
	}
	else if (j.is_array())
	{
		return testArray(j);
	}
	return ValueType::null;
}

ValueType Parser::testArray(const json & arr)
{
	if (arr.size() == 4 && isIntegerArray(arr))
	{
		return ValueType::SolidColor;
	}
	else if (arr.size() >= 4 && arr.size() % 2 == 0 && isIntegerArray(arr))
	{
		return ValueType::Points;
	}
	return ValueType::null;
}

ValueType Parser::testObject(const json & obj)
{
	return isPolygon(obj) ? ValueType::Polygon : ValueType::null;
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

bool Parser::isSolidColor(const json & arr)
{
	if (arr.size() != 4)
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

bool Parser::isPolygon(const json & obj, const std::string &polygonName)
{
	json side0, side1, solidColor;
	try { side0 = obj.at("Side0"); }catch(...) { throw Exception(fmt::format("[{}.Side0] not found in file [{}]", polygonName, m_parsingFileName)); }
	try { side1 = obj.at("Side1"); }catch(...) { throw Exception(fmt::format("[{}.Side1] not found in file [{}]", polygonName, m_parsingFileName)); }
	try { solidColor = obj.at("SolidColor"); }catch(...) { throw Exception(fmt::format("[{}.SolidColor] not found in file [{}]", polygonName, m_parsingFileName)); }
	bool sizeEqual = side0.size() == side1.size();

	bool ret = false;
	try { ret = isPoints(side0); }
	catch (InvalidArrayValueException &e)	{ 
		float x = side0[e.InvalidIndex];
		throw Exception(fmt::format("[{}.Side0] must be a int array as [Points Type], invalid value index=[{}]", polygonName, e.InvalidIndex)); 
	}
	catch (InvalidArraySizeException &e)	{ throw Exception(fmt::format("[{}.Side0].size must be a multiple of 2", polygonName)); }

	try { ret &= isPoints(side1); }
	catch (InvalidArrayValueException &e)	{ throw Exception(fmt::format("[{}.Side1] must be a int array as [Points Type], invalid value index=[{}]", polygonName, e.InvalidIndex)); }
	catch (InvalidArraySizeException &e)	{ throw Exception(fmt::format("[{}.Side1].size must be a multiple of 2", polygonName)); }

	try { ret &= isSolidColor(solidColor); }
	catch (InvalidArrayValueException &e) { throw Exception(fmt::format("[{}.SolidColor] must be a int array as [SolidColor Type], invalid value index=[{}]", polygonName, e.InvalidIndex)); }
	catch (InvalidArraySizeException &e) { throw Exception(fmt::format("[{}.SolidColor].size must be 4 as [SolidColor Type]", polygonName)); }

	if (!sizeEqual)
	{
		throw Exception(fmt::format("[{}.Side0.size={}], [{}.Side1.size={}] are not same in file [{}]", polygonName, side0.size(), polygonName, side1.size(), m_parsingFileName));
	}
	return ret;
}

bool Parser::isIntegerArray(const json & arr)
{
	for (size_t i = 0; i != arr.size(); ++i)
	{
		if (!arr[i].is_number_integer())
			throw InvalidArrayValueException(i);
	}
	return true;
}
