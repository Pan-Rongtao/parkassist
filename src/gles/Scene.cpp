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
	DrawingContext::get()->clear();
	for (auto polygon : m_polygons)
	{
		polygon->draw();
	}
	DrawingContext::get()->renderAll();
}
