#pragma once
#include <string>
#include "parkassist/gles/Def.h"
#include "nlohmann/json.hpp"
#include "parkassist/gles/fwd.h"

using nlohmann::json;

namespace nb {

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