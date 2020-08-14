#include "catch2/catch.hpp"
#include "Window.h"
#include "Common.h"
#include "parkassist/gles/fwd.h"
#include "AM8DATA.h"

using namespace nb;

#define PROJECT_NAME	"AM8"
#define WIDTH 1280
#define HEIGHT 800

TEST_CASE(PROJECT_NAME, std::string("[") + PROJECT_NAME + "]")
{
	Window w(WIDTH, HEIGHT, PROJECT_NAME);
	std::shared_ptr<Scene> sc = std::make_shared<Scene>(WIDTH, HEIGHT);
	
	w.ResizeEvent += [&w, &sc](const Window::Size &sz)
	{
		sc->doRender();
		w.swapBuffers();
	};

	int state = 0;
	auto polygonBG = Common::getBackground(std::string("../etc/") + PROJECT_NAME + ".bmp", WIDTH, HEIGHT);
	sc->add(polygonBG);
	auto polygons = Common::getPolygons(pointsAM8_R[state]);
	for (auto const p : polygons)
	{
		sc->add(p);
	}
	sc->doRender();
	w.swapBuffers();

	w.KeyEvent += [&w, &sc, polygonBG, &state](const int &key)
	{
		switch (key)
		{
		case 263:
		{
			if (state - 1 <= -((int)pointsAM8_L.size()))	return;

			--state;
			auto polygons = state >= 0 ? Common::getPolygons(pointsAM8_R[state]) : Common::getPolygons(pointsAM8_L[-state]);
			sc->clear();
			sc->add(polygonBG);
			for (auto const p : polygons)
			{
				sc->add(p);
			}
		}
		break;
		case 262:
		{
			if (state + 1 >= (int)pointsAM8_R.size())	return;

			++state;
			auto polygons = state >= 0 ? Common::getPolygons(pointsAM8_R[state]) : Common::getPolygons(pointsAM8_L[-state]);
			sc->clear();
			sc->add(polygonBG);
			for (auto const p : polygons)
			{
				sc->add(p);
			}
		}
		break;
		case 32:
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
