#pragma once
#include "parkassist/gles/Shape.h"

namespace nb {

class NB_API Scene
{
public:
	Scene(int width, int height);
	~Scene() = default;

	//添加一个多边形
	void add(MeshPtr mesh);

	//清除
	void clear();

	//执行渲染（渲染所有多边形）
	void doRender();

	//
	void enableBorder(bool enable);
	bool isBorderEnable() const;

private:
	void draw(const std::vector<MeshPtr> meshes);

	std::vector<MeshPtr> m_meshes;
	std::vector<MeshPtr> m_meshesBorder;
	bool m_enableBorder;
	CameraPtr m_camera;
};

using ScenePtr = std::shared_ptr<Scene>;
}