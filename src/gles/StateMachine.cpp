#include "parkassist/gles/StateMachine.h"
#include "parkassist/gles/Polygon.h"
#include "parkassist/gles/DrawingContext.h"

using namespace nb;

void State::addPolygon(PolygonPtr polygon)
{
	m_polygons.push_back(polygon);
}

void State::clear()
{
	m_polygons.clear();
}

int StateMachine::getCurrentState() const
{
	return m_currentState;
}

//////////////////
bool StateMachine::gotoState(uint32_t index)
{
	if (index >= m_states.size())
	{
		return false;
	}
	
	DrawingContext::get()->clear();
	for (auto polygon : m_states[index]->m_polygons)
	{
		polygon->draw();
	}
	DrawingContext::get()->renderAll();
	m_currentState = index;
	return true;
}

void StateMachine::addState(StatePtr state)
{
	m_states.push_back(state);
}

void StateMachine::removeState(uint32_t index)
{
	if (index >= m_states.size())
	{
		nbThrowException(std::out_of_range, "index[%d] is out of range[0, %d)", index, m_states.size());
	}
	m_states.erase(m_states.begin() + index);
}

uint32_t StateMachine::stateCount() const
{
	return m_states.size();
}

StatePtr StateMachine::getState(uint32_t index)
{
	if (index >= m_states.size())
	{
		nbThrowException(std::out_of_range, "index[%d] is out of range[0, %d)", index, m_states.size());
	}
	return m_states[index];
}

void StateMachine::clear()
{
	m_states.clear();
}
