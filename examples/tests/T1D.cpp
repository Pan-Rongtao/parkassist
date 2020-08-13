#include "catch2/catch.hpp"
#include "Window.h"
#include "Common.h"
#include "parkassist/gles/fwd.h"
#include "T1DDATA.h"

using namespace nb;

#define PROJECT_NAME	"T1D"
#define WIDTH 1280
#define HEIGHT 800

TEST_CASE(PROJECT_NAME, std::string("[") + PROJECT_NAME + "]")
{
	Window w(WIDTH, HEIGHT, PROJECT_NAME);
	std::shared_ptr<Scene> sc = std::make_shared<Scene>(WIDTH, HEIGHT);

	int state = 0;
	auto polygonBG = Common::getBackground(std::string("../etc/") + PROJECT_NAME + ".bmp", WIDTH, HEIGHT);
	sc->add(polygonBG);
	auto polygons = Common::getPolygons(pointsT1D_R[state]);
	for (auto const p : polygons)
	{
		sc->add(p);
	}
	sc->doRender();
	w.swapBuffers();

	w.ResizeEvent += [&w, &sc](const Window::Size &sz) { sc->doRender(); w.swapBuffers(); };
	w.KeyEvent += [&w, &sc, polygonBG, &state](const int &key)
	{
		switch (key)
		{
		case GLFW_KEY_LEFT:
		{
			if (state - 1 <= -((int)pointsT1D_L.size()))	return;

			--state;
			auto polygons = state >= 0 ? Common::getPolygons(pointsT1D_R[state]) : Common::getPolygons(pointsT1D_L[-state]);
			sc->clear();
			sc->add(polygonBG);
			for (auto const p : polygons)
			{
				sc->add(p);
			}
		}
		break;
		case GLFW_KEY_RIGHT:
		{
			if (state + 1 >= (int)pointsT1D_R.size())	return;

			++state;
			auto polygons = state >= 0 ? Common::getPolygons(pointsT1D_R[state]) : Common::getPolygons(pointsT1D_L[-state]);
			sc->clear();
			sc->add(polygonBG);
			for (auto const p : polygons)
			{
				sc->add(p);
			}
		}
		break;
		case GLFW_KEY_SPACE:
		{
			sc->enableBorder(!sc->isBorderEnable());
		}
		break;
		default:	break;
		}
		sc->doRender();
		w.swapBuffers();
	};


	while (true)
	{
		Window::pollEvents();
	}
}