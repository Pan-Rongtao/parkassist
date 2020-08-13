#pragma once
#ifndef WIN32
#include "xf86drm.h"
#include "xf86drmMode.h"
#include "gbm/gbm.h"

class EGL
{
public:
	static void init();
	static void deinit();

	static void swapBuffers();

private:
	static void initEGL();
	static void deinitEGL();
	static void initDRM();
	static void initGBM();
	static void deinitDRM();
	static void deinitGBM();
};

#endif