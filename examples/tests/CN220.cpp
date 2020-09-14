#include "catch2/catch.hpp"
#include "Window.h"
#include "Common.h"
#include "CN220DATA.h"

using namespace nb;

#define PROJECT_NAME	"CN220"
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
	auto polygonBG = Common::getBackground(std::string("../etc/CN220/BKG.bmp"), WIDTH, HEIGHT);
	sc->add(polygonBG);
	auto polygons = Common::getPolygons(HEIGHT, pointsCN220_R[state]);
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
			if (state - 1 <= -((int)pointsCN220_L.size()))	return;

			--state;
			auto polygons = state >= 0 ? Common::getPolygons(HEIGHT, pointsCN220_R[state]) : Common::getPolygons(HEIGHT, pointsCN220_L[-state]);
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
			if (state + 1 >= (int)pointsCN220_R.size())	return;

			++state;
			auto polygons = state >= 0 ? Common::getPolygons(HEIGHT, pointsCN220_R[state]) : Common::getPolygons(HEIGHT, pointsCN220_L[-state]);
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
