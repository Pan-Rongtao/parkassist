#pragma once
#include <memory>
#include "newbrush/gles/Viewport2D.h"
#include "Window.h"

namespace nb {

class Application
{
public:
	Application();
	~Application();

	int run(int argc, char *argv[]);

	static Application *current();

	static Viewport2D drawContext;

	std::vector<Window *> &windows();

private:
	static Application	*g_app;
	std::vector<Window *> m_windows;
};

}