#include "parkassist/Scene.h"
#ifdef WIN32
#include <glad/glad.h>
#else
#include <GLES2/gl2.h>
#endif
#include "parkassist/Log.h"
#include<fstream>
#include "parkassist/Bitmap.h"
//#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

using namespace nb;

Scene::Scene(float width, float height)
	: m_camera(std::make_shared<Camera>())
	, m_enableBorder(false)
	, m_enableBlend(true)
	, m_width(width)
	, m_height(height)
	, m_clearColor(0.0f, 0.0f, 0.0f, 0.0f)
{
	m_camera->ortho(0.0f, width, height, 0.0f, -1000.0f, 1000.0f);
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
#ifdef WIN32
	if (m_enableBlend)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	else
	{
		glDisable(GL_BLEND);
	}
#else
	glDisable(GL_BLEND);
#endif
	glClearColor(m_clearColor.red(), m_clearColor.green(), m_clearColor.blue(), m_clearColor.alpha());
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (m_enableBorder)
	{
		draw(m_meshesBorder);
	}
	else
	{
		draw(m_meshes);
	}
}

int Scene::width() const 
{
	return m_width;
}

int Scene::height() const 
{
	return m_height;
}

void Scene::enableBorder(bool enable)
{
	m_enableBorder = enable;
	if (enable)
	{
		for (auto mesh : m_meshes)
		{
			if (nb::is<Polygon>(mesh))
			{
				auto polygonBorder = std::make_shared<Polygon>(*nb::as<Polygon>(mesh));
				polygonBorder->mode = GL_POINTS;
				m_meshesBorder.push_back(polygonBorder);
			}
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

void Scene::setClearColor(const Color& color)
{
	m_clearColor = color;
}

void Scene::writePNG(const char* fileName) {
	m_enableBlend = false;
	doRender();

	GLubyte* pPixelData;
	GLint i, j, k;
	int channel = 4;
	GLint PixelDataLength;

	//glReadBuffer(GL_FRONT);

	//GLint viewport[4];
	//glGetIntegerv(GL_VIEWPORT, viewport);

	i = width() * channel;
	while (i % 4 != 0)
		++i;

	PixelDataLength = i * height();

	pPixelData = (GLubyte*)malloc(PixelDataLength);
	if (pPixelData == 0)
		exit(0);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glReadPixels(0, 0, width(), height(), GL_RGBA, GL_UNSIGNED_BYTE, pPixelData);

	// Write image Y-flipped because OpenGL
	stbi_write_png(fileName,
		width(), height(), 4,
		pPixelData + (width() * 4 * (height() - 1)),
		-width() * 4);

	free(pPixelData);
	m_enableBlend = true;
}

void Scene::saveFile(const std::string &fileName)
{
	m_enableBlend = false;
	doRender();

	static std::ofstream fout(fileName);
	
	GLubyte* pPixelData;
	GLint PixelDataLength;

	GLint i;
	int channel = 4;
	
	//glReadBuffer(GL_FRONT);

	//GLint viewport[4];
	//glGetIntegerv(GL_VIEWPORT, viewport);

	i = width() * channel;
	while (i % 4 != 0)
		++i;

	PixelDataLength = i * height();
	pPixelData = (GLubyte*)malloc(PixelDataLength);
	if (pPixelData == 0)
		exit(0);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glReadPixels(0, 0, width(), height(), GL_RGBA, GL_UNSIGNED_BYTE, pPixelData);

	for (int m = 0; m < height() / 2; m++) {
		for (int n = 0; n < i; n++) {
			std::swap(pPixelData[i * m + n], pPixelData[i * (height() - m - 1) + n]);
		}
	}

	for (int k = 0; k < height(); k++) {
		for (int m = 0; m < i; m++) {
			fout << (int)pPixelData[i * k + m] << " ";
		}
		fout << std::endl;
	}
	fout << std::flush;
	fout.close();
	free(pPixelData);

	m_enableBlend = true;
}

void Scene::saveFrameBuffer(std::string& buffer)
{
	m_enableBlend = false;
	doRender();

	GLubyte* pPixelData;
	GLint PixelDataLength;

	GLint i;
	int channel = 4;
	
	//glReadBuffer(GL_FRONT);
	
	//GLint viewport[4];
	//glGetIntegerv(GL_VIEWPORT, viewport);

	i = width() * channel; 
	while (i % 4 != 0)
		++i;

	PixelDataLength = i * height();
	pPixelData = (GLubyte*)malloc(PixelDataLength);
	if (pPixelData == 0)
		exit(0);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glReadPixels(0, 0, width(), height(), GL_RGBA, GL_UNSIGNED_BYTE, pPixelData);

	for (int m = 0; m < height() / 2; m++) {
		for (int n = 0; n < i; n++) {
			std::swap(pPixelData[i * m + n], pPixelData[i * (height() - m - 1) + n]);
		}
	}

	buffer = std::string((char*)pPixelData, PixelDataLength);

	free(pPixelData);

	m_enableBlend = true;
}

void Scene::draw(const std::vector<MeshPtr> meshes)
{
	static int frames = 0;
	static auto t0 = nb::getTickCount();
	auto pt = nb::getTickCount();
	for (auto mesh : meshes)
	{
		mesh->draw(m_camera);
	}

	++frames;
	auto t1 = nb::getTickCount();
	if (t1 - t0 >= 2000)
	{
		auto fps = frames * 1000.0 / (t1 - t0);
		frames = 0;
		t0 = t1;
		Log::info("fps={}, fc={}, last frame cost={} ms", fps, 1000.0 / fps, t1 - pt);
	}

}
