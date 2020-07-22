#include "parkassist/gles/Polygon.h"
#include "parkassist/gles/DrawingContext.h"
#include "parkassist/gles/Brush.h"
#include "parkassist/gles/Color.h"
#include <GLES2/gl2.h>

using namespace nb;

Polygon::Polygon()
	: Polygon({}, {})
{
}

Polygon::Polygon(const std::vector<glm::vec2>& side0, const std::vector<glm::vec2>& side1)
	: m_side0(side0)
	, m_side1(side1)
	, m_controlPointsCount(10)
	, m_sampleCount(50)
	, m_brush(std::make_shared<SolidColorBrush>(Colors::black()))
	, m_drawMode(GL_TRIANGLES)
{
}

void Polygon::setSide0(const std::vector<glm::vec2>& side0)
{
	m_side0 = side0;
}

void Polygon::setSide1(const std::vector<glm::vec2>& side1)
{
	m_side1 = side1;
}

const std::vector<glm::vec2>& Polygon::side0() const
{
	return m_side0;
}

const std::vector<glm::vec2>& Polygon::side1() const
{
	return m_side1;
}

void Polygon::setBezierParams(int controlPointsCount, int sampleCount)
{
	m_controlPointsCount = controlPointsCount;
	m_sampleCount = sampleCount;
}

int Polygon::controlPointsCount() const
{
	return m_controlPointsCount;
}

int Polygon::sampleCount() const
{
	return m_sampleCount;
}

void Polygon::setBrush(BrushPtr brush)
{
	m_brush = brush;
}

BrushPtr Polygon::brush() const
{
	return m_brush;
}

void Polygon::setMode(uint8_t mode)
{
	m_drawMode = mode;
}
uint8_t Polygon::mode() const
{
	return m_drawMode;
}
//
//void Polygon::draw()
//{
//	DrawingContext::get()->drawPolygon(m_side0, m_side1, m_brush, m_controlPointsCount, m_sampleCount, m_drawMode);
//}
