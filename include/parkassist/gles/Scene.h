#pragma once
#include "parkassist/gles/Polygon.h"
#include "parkassist/gles/DrawingContext.h"

namespace nb {

class NB_API Scene
{
public:
	Scene(int width, int height);
	~Scene() = default;

	//添加一个多边形
	void add(std::shared_ptr<Polygon> polygon);

	//清除
	void clear();

	//执行渲染（渲染所有多边形）
	void doRender();

	//
	void enableBorder(bool enable);
	bool isBorderEnable() const;

private:
	void draw(const std::vector<std::shared_ptr<Polygon>> polygons);

	DrawingContext m_dc;
	std::vector<std::shared_ptr<Polygon>> m_polygons;
	std::vector<std::shared_ptr<Polygon>> m_polygonsBorder;
	bool m_enableBorder;
};

using ScenePtr = std::shared_ptr<Scene>;
}