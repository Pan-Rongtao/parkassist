#pragma once
#include <string>
#include "newbrush/gles/Def.h"
#include "nlohmann/json.hpp"

using nlohmann::json;

namespace nb {

class RenderObject;
using RenderObjectPtr = std::shared_ptr<RenderObject>;
using DrawingState = std::vector<RenderObjectPtr>;

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

	const std::vector<DrawingState> &drawingStates() const;

private:
	void parseConfig();
	void parseStates();

	json parseOneFile(const std::string &fileName);

	DrawingState makeDrawingState(const json &obj);

	bool isPoints(const json &arr);
	bool isSolidColor(const json &arr);
	bool isPolygon(const json &obj, const std::string &polygonName = "");
	bool isIntegerArray(const json &arr);

	std::string m_dir;
	json m_commonCfg;

	int m_contextWidth;
	int m_contextHeight;
	int m_stateCount;
	std::vector<DrawingState>	m_drawingStates;
	std::string m_parsingFileName;
};

}