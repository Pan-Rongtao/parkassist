#include "parkassist/gles/Scene.h"
#include <GLES2/gl2.h>

using namespace nb;

Scene::Scene(int width, int height)
	: m_enableBorder(false)
{
	m_dc.resize(width, height);
}

void Scene::add(std::shared_ptr<Polygon> polygon)
{
	m_polygons.push_back(polygon);
}

void Scene::clear()
{
	m_polygons.clear();
	m_polygonsBorder.clear();
}

void Scene::doRender()
{
	m_dc.clear();
	if (m_enableBorder)
	{
		draw(m_polygonsBorder);
	}
	else
	{
		draw(m_polygons);
	}
	m_dc.renderAll();
}

void Scene::enableBorder(bool enable)
{
	m_enableBorder = enable;
	if (enable)
	{
		for (auto p : m_polygons)
		{
			auto polygonBorder = std::make_shared<Polygon>(*p);
			polygonBorder->setMode(GL_POINTS);
			m_polygonsBorder.push_back(polygonBorder);
		}
	}
	else
	{
		m_polygonsBorder.clear();
	}
}

bool Scene::isBorderEnable() const
{
	return m_enableBorder;
}

void Scene::draw(const std::vector<std::shared_ptr<Polygon>> polygons)
{
	for (auto polygon : polygons)
	{
		m_dc.drawPolygon(polygon->side0(), polygon->side1(), polygon->brush(),
			polygon->controlPointsCount(), polygon->sampleCount(), polygon->mode());
	}

}
