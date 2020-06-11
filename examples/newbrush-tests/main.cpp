#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"
#include <GLES2/gl2.h>
#include "GLFW/glfw3.h"
#include "newbrush/gles/Viewport2D.h"
#include "newbrush/gles/Strips.h"
#include "newbrush/gles/Program.h"
#include "newbrush/gles/RenderObject.h"
#include "newbrush/gles/Quadrangle.h"

using namespace nb;

GLFWwindow *m_implWindow;
Viewport2D m_viewport;

void init()
{
	glfwSetErrorCallback([](int error, const char*str) { printf("error:%s\n", str); });
	glfwInit();
	//以下两句在有些电脑上导致glfwDestroyWindow挂死，放在库外不会挂死，目前暂未找到原因
	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
	glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_EGL_CONTEXT_API);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
}

TEST_CASE("1: All test cases reside in other .cpp files (empty)", "[multi-file:1]") 
{
	init();

	auto w = 800;
	auto h = 600;
	m_implWindow = glfwCreateWindow(800, 600, "newbrush", nullptr, nullptr);
	glfwMakeContextCurrent(m_implWindow);
	m_viewport.projection.ortho(0.0f, w, h, 0.0f, -1000.0f, 1000.0f);
	//m_viewport.projection.perspective(100.0f, (float)w / h, 0.1f, 10000.0f);
	//m_viewport.camera.lookat2D((float)w, (float)h);
	m_viewport.viewport(0, 0, w, h);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	glm::vec2 p0, p1, p2, p3;
	p0 = { 200, 500 };
	p1 = { 300, 500 };
	p2 = { 300, 100 };
	p3 = { 200, 100 };
	auto quad = std::make_shared<Quadrangle>(p0, p1, p2, p3);
	auto renderObj = std::make_shared<RenderObject>(quad, Programs::primitive());
	renderObj->storeUniform("color", glm::vec4(1.0, 0.0, 0.0, 1.0));
	m_viewport.queue(renderObj);
	
	while (1)
	{
		glfwMakeContextCurrent(m_implWindow);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		m_viewport.draw();

		glfwSwapBuffers(m_implWindow);

		glfwPollEvents();
	}

}
