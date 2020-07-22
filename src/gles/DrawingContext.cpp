#include "parkassist/gles/DrawingContext.h"
#include "parkassist/gles/Renderer.h"
#include "parkassist/gles/Camera.h"
#include "parkassist/gles/PolygonGeometry.h"
#include "parkassist/gles/Brush.h"
#include "parkassist/gles/ImageSource.h"
#include "parkassist/gles/Texture2D.h"
#include <GLES2/gl2.h>

using namespace nb;

DrawingContext::DrawingContext()
	: m_camera(std::make_shared<Camera>())
{
}

DrawingContext* DrawingContext::get()
{
	static auto p = new DrawingContext();
	return p;
}

CameraPtr DrawingContext::camera()
{
	return m_camera;
}

void DrawingContext::resize(int width, int height)
{
	glViewport(0, 0, width, height);
	m_camera->ortho(0.0f, (float)width, (float)height, 0.0f, -1000.0f, 1000.0f);
}

void DrawingContext::drawPolygon(const std::vector<glm::vec2>& side0, const std::vector<glm::vec2>& side1, BrushPtr brush, int controlPointsCount, int sampleCount, uint8_t drawMode)
{
	if (!brush)
	{
		return;
	}
	
	auto geometry = std::make_shared<PolygonGeometry>();
	geometry->setPoint(side0, side1, controlPointsCount, sampleCount);
	auto renderer = std::make_shared<Renderer>(geometry);
	geometry->mode = drawMode;
	if (nb::is<SolidColorBrush>(brush))
	{
		renderer->setProgram(Programs::primitive());

		auto const &color = nb::as<SolidColorBrush>(brush)->color();
		auto colorF = glm::vec4(color.redF(), color.greenF(), color.blueF(), color.alphaF());
		renderer->mesh()->unifyColor(colorF);
		m_renderers.push_back(renderer);
	}
	else if (nb::is <LinearGradientBrush>(brush))
	{
		renderer->setProgram(Programs::gradientPrimitive());

		auto const &stops = nb::as<LinearGradientBrush>(brush)->gradientStops();
		std::vector<glm::vec4> colors;
		std::vector<float> offsets;
		for (auto const one : stops)
		{
			colors.push_back({ one.color.redF(), one.color.greenF(), one.color.blueF(), one.color.alphaF() });
			offsets.push_back(one.offset);
		}
		renderer->storeUniform("size", colors.size());
		renderer->storeUniform("colors", colors);
		renderer->storeUniform("offsets", offsets);
		m_renderers.push_back(renderer);
	}
	else if (nb::is<ImageBrush>(brush))
	{
		geometry->vertexs[0].texCoord = glm::vec2(0.0, 1.0);
		geometry->vertexs[1].texCoord = glm::vec2(1.0, 1.0);
		geometry->vertexs[2].texCoord = glm::vec2(0.0, 0.0);
		geometry->vertexs[3].texCoord = glm::vec2(1.0, 0.0);
		renderer->setProgram(Programs::image());
		auto source = nb::as<ImageBrush>(brush)->source();
		if (source)
		{
			auto const &stream = source->stream();
			auto texture = std::make_shared<Texture2D>();
			auto glFormatAndType = Texture::getGlFormatAndType(source->channels());
			texture->update((const unsigned char *)stream.data(), (int)source->width(), (int)source->height(), glFormatAndType.first, glFormatAndType.second);
			geometry->material.textures().push_back(texture);
		}
		m_renderers.push_back(renderer);
	}
}

void DrawingContext::drawImage(ImageSourcePtr source, const Rect & rect)
{
}

void DrawingContext::clear()
{
	m_renderers.clear();
}

void DrawingContext::renderAll()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	for (auto const &renderer : m_renderers)
	{
		renderer->draw(m_camera);
	}
}
