#include "parkassist/gles/Scene.h"
#include <GLES2/gl2.h>

using namespace nb;

Scene::Scene(int width, int height)
	: m_camera(std::make_shared<Camera>())
	, m_enableBorder(false)
{
	glViewport(0, 0, width, height);
	m_camera->ortho(0.0f, (float)width, (float)height, 0.0f, -1000.0f, 1000.0f);
}

void Scene::add(MeshPtr mesh)
{
	m_meshes.push_back(mesh);
}

void Scene::clear()
{
	m_meshes.clear();
	m_meshesBorder.clear();
}

void Scene::doRender()
{
	if (m_enableBorder)
	{
		draw(m_meshesBorder);
	}
	else
	{
		draw(m_meshes);
	}
}

void Scene::enableBorder(bool enable)
{
	m_enableBorder = enable;
	if (enable)
	{
		for (auto p : m_meshes)
		{
			auto polygonBorder = std::make_shared<Polygon>(*nb::as<Polygon>(p));
			polygonBorder->mode = GL_POINTS;
			m_meshesBorder.push_back(polygonBorder);
		}
	}
	else
	{
		m_meshesBorder.clear();
	}
}

bool Scene::isBorderEnable() const
{
	return m_enableBorder;
}

void Scene::draw(const std::vector<MeshPtr> polygons)
{
	for (auto polygon : polygons)
	{
		polygon->draw(m_camera);
	}

}
