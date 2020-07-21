#pragma once
#include "parkassist/gles/Def.h"

namespace nb {
	
class Polygon;
using PolygonPtr = std::shared_ptr<Polygon>;
class NB_API State
{
public:
	void addPolygon(PolygonPtr polygon);
	void clear();
	
private:
	std::vector<PolygonPtr> m_polygons;
	friend class StateMachine;
};

using StatePtr = std::shared_ptr<State>;
class NB_API StateMachine
{
public:
	//获取当前状态
	int getCurrentState() const;

	//切换到状态
	bool gotoState(uint32_t index);

	void addState(StatePtr state);
	void removeState(uint32_t index);

	uint32_t stateCount() const;
	StatePtr getState(uint32_t index);
	void clear();

private:
	std::vector<StatePtr> m_states;
	int m_currentState{-1};
};

}