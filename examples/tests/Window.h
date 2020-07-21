#pragma once
#include <string>
#include "GLFW/glfw3.h"
#include "Event.h"

struct GLFWwindow;
namespace nb {

class Window
{
public:
	Window(int width, int height, const std::string &title);
	~Window();

	void swapBuffers();

	void resize(int width, int height);

	static void pollEvents();

	Event<int>	KeyEvent;
	struct Size { int width; int height; };
	Event<Size>	ResizeEvent;

private:
	static void init();

	void sizeCallback(int width, int height);
	void keyCallback(int key, int scancode, int action, int mods);

	GLFWwindow *m_implWindow;
};

}