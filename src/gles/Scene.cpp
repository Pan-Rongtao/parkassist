#include "parkassist/gles/Scene.h"

using namespace nb;

Scene::Scene(int width, int height)
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
}

void Scene::doRender()
{
	m_dc.clear();
	for (auto polygon : m_polygons)
	{
		m_dc.drawPolygon(polygon->side0(), polygon->side1(), polygon->brush(), 
			polygon->controlPointsCount(), polygon->sampleCount(), polygon->mode());
	}
	m_dc.renderAll();
}
