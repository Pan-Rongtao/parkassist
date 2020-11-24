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

	//颜色输出到.bmp
	void SaveToBMP(const char* fileName);

	//颜色输出到文本文件
	void SaveToFile(const std::string &fileName);

	//颜色输出到buffer，按RGBA顺序
	void SaveToFrameBuffer(std::string& buffer);

private:
	void draw(const std::vector<MeshPtr> meshes);

	std::vector<MeshPtr> m_meshes;
	std::vector<MeshPtr> m_meshesBorder;
	bool m_enableBorder;
	CameraPtr m_camera;
	int m_width, m_height;
};

using ScenePtr = std::shared_ptr<Scene>;
}