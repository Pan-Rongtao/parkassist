#include "Application.h"
#include <GLES2/gl2.h>
#include "Timer.h"

using namespace nb;

Application *Application::g_app = nullptr;
Viewport2D Application::drawContext;

Application::Application()
{
	if (g_app)
	{
		assert(1);
	}
	g_app = this;
}

Application::~Application()
{
}

int Application::run(int argc, char * argv[])
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	while (1)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		drawContext.draw();
		for (auto window : m_windows)
		{
			window->render();
		}
		Timer::driveInLoop();
		Window::pollEvents();
	}

	return 0;
}

Application * Application::current()
{
	return g_app;
}

std::vector<Window*>& Application::windows()
{
	return m_windows;
}
