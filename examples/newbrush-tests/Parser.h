#pragma once
#include <string>
#include "newbrush/gles/Def.h"
#include "nlohmann/json.hpp"

using nlohmann::json;

namespace nb {

class RenderObject;
using RenderObjectPtr = std::shared_ptr<RenderObject>;
using DrawingState = std::vector<RenderObjectPtr>;

class NB_API Parser
{
public:
	void setDir(const std::string &dir);
	std::string getDir() const;

	bool parse();



private:
	bool parseCommonCfg();
	bool parseDrawingStates();

	bool parseOneFile(const std::string &path, json &obj);
	DrawingState makeToDrawingState(json obj);

	std::string m_dir;
	json m_commonCfg;

	int m_stateCount;
	std::vector<DrawingState>	m_drawingStates;

};

}