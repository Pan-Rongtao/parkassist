#include "Window.h"
#include "Application.h"

using namespace nb;

Window::Window(int width, int height, const std::string &title)
{
	init();
	m_implWindow = glfwCreateWindow(width, height, title.data(), 0, 0);
	glfwMakeContextCurrent(m_implWindow);

	glfwSetWindowUserPointer(m_implWindow, this);
	glfwSetWindowSizeCallback(m_implWindow, [](GLFWwindow*w, int width, int height) { static_cast<Window *>(glfwGetWindowUserPointer(w))->sizeCallback(width, height); });
	glfwSetKeyCallback(m_implWindow, [](GLFWwindow*w, int key, int scancode, int action, int mods) { 
		auto p = glfwGetWindowUserPointer(w);
		auto pp = static_cast<Window *>(p);
		pp->keyCallback(key, scancode, action, mods);
	});

	sizeCallback(width, height);
	Application::current()->windows().push_back(this);
}

Window::~Window()
{
	auto &windows = Application::current()->windows();
	auto iter = std::find(windows.begin(), windows.end(), this);
	if (iter != windows.end())
	{
		Application::current()->windows().erase(iter);
	}
}

void Window::render()
{
	glfwSwapBuffers(m_implWindow);
}

void Window::resize(int width, int height)
{
	glfwSetWindowSize(m_implWindow, width, height);
}

void Window::pollEvents()
{
	glfwPollEvents();
}

void Window::init()
{
	static bool bInit = false;
	if (bInit)	return;

	glfwSetErrorCallback([](int error, const char*str) { printf("error:%s\n", str); });
	glfwInit();
	//以下两句在有些电脑上导致glfwDestroyWindow挂死，放在库外不会挂死，目前暂未找到原因
	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
	glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_EGL_CONTEXT_API);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_SAMPLES, 32);
	bInit = true;
}

void Window::sizeCallback(int width, int height)
{
	auto app = Application::current();
	if (app)
	{
		app->drawContext.projection.ortho(0.0f, (float)width, (float)height, 0.0f, -1000.0f, 1000.0f);
		app->drawContext.viewport(0, 0, width, height);
	}
}

void Window::keyCallback(int key, int scancode, int action, int mods)
{
	if(action == 1 || action == GLFW_REPEAT)
		KeyEvent.invoke({key});
}