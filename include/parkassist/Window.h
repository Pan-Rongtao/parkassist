#pragma once
#include <string>
#include "parkassist/Event.h"
#include "parkassist/Size.h"
#ifndef WIN32
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include "xf86drm.h"
#include "xf86drmMode.h"
#include "gbm/gbm.h"
#include "GLES2/gl2.h"
#include "EGL/egl.h"
#endif


struct GLFWwindow;
namespace nb {

class NB_API Window
{
public:
	Window(float width, float height, const std::string &title);
	~Window();

	void swapBuffers();

	void resize(float width, float height);

	float width() const;
	float height() const;

	static void pollEvents();

	Event<int>	KeyEvent;
	Event<Size>	ResizeEvent;

private:
	static void init();

	void sizeCallback(int width, int height);
	void keyCallback(int key, int scancode, int action, int mods);

#ifdef WIN32
	GLFWwindow *m_implWindow;
#else

	static int initEGL();
	static int initDRM();
	static int initGBM();
	void deinitEGL();
	void deinitDRM();
	void deinitGBM();

#endif // WIN23

};

}
