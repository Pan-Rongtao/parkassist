#pragma once
#include <string>
#include "parkassist/gles/Def.h"
#include "nlohmann/json.hpp"

using nlohmann::json;

namespace nb {

class Renderer;
using RendererPtr = std::shared_ptr<Renderer>;
using DrawingState = std::vector<RendererPtr>;

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


	void enablePointsObjects(bool bOn);
	bool isPointsObjectsOn() const;

	int drawingStatesCount() const;
	DrawingState getDrawingState(int index) const;

private:
	void parseConfig();
	void parseStates();

	json parseOneFile(const std::string &fileName);

	std::pair<DrawingState, DrawingState> makeDrawingState(const json &obj);

	bool isPoints(const json &arr);
	bool isSolidColor(const json &arr);
	bool isPolygon(const json &obj, const std::string &polygonName = "");
	bool isIntegerArray(const json &arr);

	std::string m_dir;
	json m_commonCfg;

	int m_contextWidth;
	int m_contextHeight;
	int m_stateCount;
	int m_bezierControlPointsCount;
	int m_bezierSampleCount;
	bool m_drawPointMode;
	std::vector<DrawingState>	m_drawingStates;
	std::vector<DrawingState>	m_drawingStates_Points;
	bool m_bEnablePiontsObject{ false };
	std::string m_parsingFileName;
};

}