#include "parkassist/Scene.h"
#include <GLES2/gl2.h>
//#include <glad/glad.h>
#include "parkassist/Log.h"
#include<fstream>
#include "parkassist/Bitmap.h"

using namespace nb;

Scene::Scene(float width, float height)
	: m_camera(std::make_shared<Camera>())
	, m_enableBorder(false)
	, m_width(width)
	, m_height(height)
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
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
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

#define BMP_Header_Length 54
void Scene::SaveToBMP(const char* fileName) {
	FILE* pDummyFile;
	FILE* pWritingFile;
	GLubyte* pPixelData;
	GLubyte* pBmpData;
	GLubyte BMP_Header[BMP_Header_Length];
	GLint i, j, k;
	int channel = 4;
	GLint PixelDataLength;
	GLint BmpDataLength;

	//glReadBuffer(GL_FRONT);

	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	i = viewport[2] * channel;
	while (i % 4 != 0)
		++i;

	k = viewport[2] * (channel - 1);
	while (k % 4 != 0)
		++k;

	PixelDataLength = i * viewport[3];
	BmpDataLength = k * viewport[3];

	pPixelData = (GLubyte*)malloc(PixelDataLength);
	if (pPixelData == 0)
		exit(0);

	pBmpData = (GLubyte*)malloc(BmpDataLength);
	if (pBmpData == 0)
		exit(0);

	pDummyFile = fopen("../etc/CN220/ground.bmp", "rb");
	if (pDummyFile == nullptr)
		exit(0);

	pWritingFile = fopen(fileName, "wb");
	if (pWritingFile == nullptr)
		exit(0);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glReadPixels(0, 0, viewport[2], viewport[3], GL_RGBA, GL_UNSIGNED_BYTE, pPixelData);

	//pPixelData��ȡ������ɫ��RGBA˳�����У����ﰴBGRȡֵ��
	for (int counter = 0; counter < viewport[2] * viewport[3]; counter++) {
		pBmpData[counter * 3 + 0] = pPixelData[counter * 4 + 2];
		pBmpData[counter * 3 + 1] = pPixelData[counter * 4 + 1];
		pBmpData[counter * 3 + 2] = pPixelData[counter * 4 + 0];
	}

	fread(BMP_Header, sizeof(BMP_Header), 1, pDummyFile);
	fwrite(BMP_Header, sizeof(BMP_Header), 1, pWritingFile);
	fseek(pWritingFile, 0x0012, SEEK_SET);
	i = viewport[2];
	j = viewport[3];
	fwrite(&i, sizeof(i), 1, pWritingFile);
	fwrite(&j, sizeof(j), 1, pWritingFile);

	fseek(pWritingFile, 0, SEEK_END);
	fwrite(pBmpData, BmpDataLength, 1, pWritingFile);

	fclose(pDummyFile);
	fclose(pWritingFile);
	free(pPixelData);
	free(pBmpData);
}

void Scene::SaveToFile(const std::string &fileName)
{
	static std::ofstream fout(fileName);
	
	GLubyte* pPixelData;
	GLint PixelDataLength;

	GLint i;
	int channel = 4;
	
	//glReadBuffer(GL_FRONT);

	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	i = viewport[2] * channel;
	while (i % 4 != 0)
		++i;

	PixelDataLength = i * viewport[3];
	pPixelData = (GLubyte*)malloc(PixelDataLength);
	if (pPixelData == 0)
		exit(0);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glReadPixels(0, 0, viewport[2], viewport[3], GL_RGBA, GL_UNSIGNED_BYTE, pPixelData);

	for (int m = 0; m < viewport[3] / 2; m++) {
		for (int n = 0; n < i; n++) {
			std::swap(pPixelData[i * m + n], pPixelData[i * (viewport[3] - m - 1) + n]);
		}
	}

	for (int k = 0; k < viewport[3]; k++) {
		for (int m = 0; m < i; m++) {
			fout << (int)pPixelData[i * k + m] << " ";
		}
		fout << std::endl;
	}
	fout << std::flush;
	fout.close();
	free(pPixelData);
}

void Scene::SaveToFrameBuffer(std::string& buffer)
{
	GLubyte* pPixelData;
	GLint PixelDataLength;

	GLint i;
	int channel = 4;	// Ĭ��4ͨ��
	
	//glReadBuffer(GL_FRONT);
	
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	i = viewport[2] * channel; 
	while (i % 4 != 0) //�ֽڶ��룬��ߴ洢Ч��
		++i;

	PixelDataLength = i * viewport[3];
	pPixelData = (GLubyte*)malloc(PixelDataLength);
	if (pPixelData == 0)
		exit(0);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glReadPixels(0, 0, viewport[2], viewport[3], GL_RGBA, GL_UNSIGNED_BYTE, pPixelData);

	//glReadPixels�õ��������Ǵ������ϣ��������ҡ����������һ�����µ��á�
	for (int m = 0; m < viewport[3] / 2; m++) {
		for (int n = 0; n < i; n++) {
			std::swap(pPixelData[i * m + n], pPixelData[i * (viewport[3] - m - 1) + n]);
		}
	}

	buffer = std::string((char*)pPixelData, PixelDataLength);
	free(pPixelData);
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
