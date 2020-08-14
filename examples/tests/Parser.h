#pragma once
#include <string>
#include <exception>
#include "parkassist/gles/Def.h"
#include "nlohmann/json.hpp"
#include "parkassist/gles/fwd.h"
#include "spdlog/fmt/fmt.h"

using nlohmann::json;

namespace nb {
	
class Exception : public std::exception
{
public:
	Exception() = default;
	Exception(const std::string &msg) : m_msg(msg) { }

protected:
	virtual char const* what() const noexcept { return m_msg.data(); }
	std::string m_msg;
};

class FileNotExistsException : public Exception
{
public:
	FileNotExistsException(const std::string &path) { m_msg = "can't find [" + path + "]"; }
};

class JsonParsingException : public Exception
{
public:
	JsonParsingException(const std::string &filePath, const std::string &errorMsg) { m_msg = fmt::format("[{}] syntax error.\n\t{}", filePath, errorMsg); }
};

class InvalidArraySizeException : public Exception
{
public:
	InvalidArraySizeException() = default;
};

class InvalidArrayValueException : public Exception
{
public:
	InvalidArrayValueException(int invalidIndex) : InvalidIndex(invalidIndex) { m_msg = fmt::format("invalidIndex({})", invalidIndex); }
	int InvalidIndex;
};

enum class ValueType
{
	null,
	boolean,
	integer,
	real,
	string,
	Polygon,
	Points,
	SolidColor,

};

class NB_API Parser
{
public:
	void setDir(const std::string &dir);
	std::string getDir() const;

	bool parse();

	int getContextWidth() const;
	int getContextHeight() const;

	int drawingStatesCount() const;
	std::vector<PolygonPtr> getDrawingState(int index) const;

private:
	void parseConfig();
	void parseStates();

	json parseOneFile(const std::string &fileName);

	std::vector<PolygonPtr> makeDrawingState(const json &obj);

	bool isPoints(const json &arr);
	BrushPtr toBrush(const json &arr);
	bool isPolygon(const json &obj, const std::string &polygonName = "");
	bool isIntegerArray(const json &arr);

	std::string m_dir;
	json m_commonCfg;

	int m_contextWidth;
	int m_contextHeight;
	int m_stateCount;
	int m_bezierControlPointsCount;
	int m_bezierSampleCount;
	std::vector<std::vector<PolygonPtr>>	m_drawingStates;
	std::string m_parsingFileName;
};

}
