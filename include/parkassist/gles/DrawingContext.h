#pragma once
#include <vector>
#include "parkassist/gles/Rect.h"
#include "glm/glm.hpp"

namespace nb {
	
class Brush;
class Renderer;
class Camera;
class ImageSource;
using BrushPtr = std::shared_ptr<Brush>;
using RendererPtr = std::shared_ptr<Renderer>;
using CameraPtr = std::shared_ptr<Camera>;
using ImageSourcePtr = std::shared_ptr<ImageSource>;

class NB_API DrawingContext
{
public:
	DrawingContext();

	CameraPtr camera();
	void resize(int width, int height);

	//绘制一个多边形
	void drawPolygon(const std::vector<glm::vec2> &side0, const std::vector<glm::vec2> &side1, BrushPtr brush, int controlPointsCount, int sampleCount, uint8_t drawMode);

	//绘制一个图片
	//绘制图片
	void drawImage(ImageSourcePtr source, const Rect &rect);

	//清除渲染物
	void clear();

	//渲染所有渲染物
	void renderAll();

private:
	CameraPtr m_camera;
	std::vector<RendererPtr> m_renderers;
	int m_width;
	int m_height;
};



}