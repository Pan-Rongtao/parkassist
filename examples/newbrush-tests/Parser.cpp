#include "Parser.h"
#include <fstream>
#include "spdlog/spdlog.h"

using namespace nb;
using namespace nlohmann;

constexpr char *CfgFile = "cfg.json";

void Parser::setDir(const std::string & dir)
{
	m_dir = dir + "/";
}

std::string Parser::getDir() const
{
	return m_dir;
}

bool Parser::parse()
{
	return parseCommonCfg() & parseDrawingStates();
}

bool Parser::parseCommonCfg()
{
	json obj;
	bool b = parseOneFile(m_dir + CfgFile, obj);
	if (!b)
	{
		return false;
	}

	try {
		m_stateCount = obj["StateCount"];
		m_drawingStates.resize(m_stateCount);
	}
	catch(std::exception &e)
	{
		spdlog::error("get 'StateCount' fail. {}", e.what());
		return false;
	}
	return true;
}

bool Parser::parseDrawingStates()
{
	for (size_t i = 0; i != m_drawingStates.size(); ++i)
	{
		std::string stateFilePath = m_dir + fmt::format("state{}.json", i);
		json obj;
		bool b = parseOneFile(stateFilePath, obj);
		if (b)
		{

		}
		else
		{
			spdlog::warn("state{} will draw nothing because of parsing error.", i);
		}
	}
	return true;
}

bool Parser::parseOneFile(const std::string & path, json & obj)
{
	std::ifstream stream(path.data());
	if (!stream)
	{
		spdlog::error("can't find [{}]", path);
		return false;
	}

	try {
		stream >> obj;
	}
	catch (std::exception &e) 
	{
		spdlog::error("parse [{}] error.\t{}", path, e.what());
		stream.close();
		return false;
	}
	return true;
}

DrawingState Parser::makeToDrawingState(json obj)
{
	for (auto item : obj)
	{
		if (item.is_object())
		{

		}
		else
		{
			spdlog::warn("{} is not a object type when parsing");
		}
	}
}
