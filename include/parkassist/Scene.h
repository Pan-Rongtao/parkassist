#pragma once
#include "parkassist/Shape.h"

namespace nb {

class NB_API Scene
{
public:
	Scene(float width, float height);
	~Scene() = default;

	//添加一个多边形
	void add(MeshPtr mesh);

	//清除
	void clear();

	//执行渲染（渲染所有多边形）
	void doRender();

	int width() const;

	int height() const;

	void enableBorder(bool enable);
	bool isBorderEnable() const;

	//设置背景色
	void setClearColor(const Color& color);

	//颜色输出到.png
	void writePNG(const char* fileName);

	//颜色输出到文本文件
	void saveFile(const std::string &fileName);

	//颜色输出到buffer，按RGBA顺序
	void saveFrameBuffer(std::string& buffer);

private:
	void draw(const std::vector<MeshPtr> meshes);

	std::vector<MeshPtr> m_meshes;
	std::vector<MeshPtr> m_meshesBorder;
	bool m_enableBorder;
	bool m_enableBlend;
	CameraPtr m_camera;
	int m_width, m_height;
	Color m_clearColor;
};

using ScenePtr = std::shared_ptr<Scene>;
}