#pragma once
#include <string>
#include "GLFW/glfw3.h"

struct GLFWwindow;
namespace nb {

class Window
{
public:
	Window(int width, int height, const std::string &title);
	~Window();

	void render();

	void resize(int width, int height);

	static void pollEvents();

private:
	static void init();

	void sizeCallback(int width, int height);

	GLFWwindow *m_implWindow;
};

}