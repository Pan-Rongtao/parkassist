#include "catch2/catch.hpp"
#include "parkassist/Window.h"
#include "Helper.h"
#include "T1D.h"

using namespace nb;

#define PROJECT_NAME	"T1D"
#define WIDTH 1280.0f
#define HEIGHT 800.0f

TEST_CASE(PROJECT_NAME, std::string("[") + PROJECT_NAME + "]")
{
	Window w(WIDTH, HEIGHT, PROJECT_NAME);
	std::shared_ptr<Scene> sc = std::make_shared<Scene>(WIDTH, HEIGHT);

	int state = 0;
	auto polygonBG = Helper::getBackground(std::string("../etc/") + PROJECT_NAME + ".bmp", WIDTH, HEIGHT);
	sc->add(polygonBG);
	auto polygons = Helper::getPolygons(HEIGHT, pointsT1D_R[state]);
	for (auto const p : polygons)
	{
		sc->add(p);
	}
	sc->doRender();
	w.swapBuffers();

	w.ResizeEvent += [&w, &sc](const Size &sz) { sc->doRender(); w.swapBuffers(); };
	w.KeyEvent += [&w, &sc, polygonBG, &state](const int &key)
	{
		switch (key)
		{
		case 263:
		{
			if (state - 1 <= -((int)pointsT1D_L.size()))	return;

			--state;
			auto polygons = state >= 0 ? Helper::getPolygons(HEIGHT, pointsT1D_R[state]) : Helper::getPolygons(HEIGHT, pointsT1D_L[-state]);
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
			if (state + 1 >= (int)pointsT1D_R.size())	return;

			++state;
			auto polygons = state >= 0 ? Helper::getPolygons(HEIGHT, pointsT1D_R[state]) : Helper::getPolygons(HEIGHT, pointsT1D_L[-state]);
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
