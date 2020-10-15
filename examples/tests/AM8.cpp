#include "catch2/catch.hpp"
#include "parkassist/Window.h"
#include "Helper.h"
#include "AM8.h"

using namespace nb;

#define PROJECT_NAME	"AM8"
#define WIDTH 1280.0f
#define HEIGHT 800.0f

TEST_CASE(PROJECT_NAME, std::string("[") + PROJECT_NAME + "]")
{
	Window w(WIDTH, HEIGHT, PROJECT_NAME);
	std::shared_ptr<Scene> sc = std::make_shared<Scene>(WIDTH, HEIGHT);
	
	w.ResizeEvent += [&w, &sc](const Size &sz)
	{
		sc->doRender();
		w.swapBuffers();
	};

	int state = 0;
	auto polygonBG = Helper::getBackground(std::string("../etc/CN220/BKG.bmp"), WIDTH, HEIGHT);
	sc->add(polygonBG);
	auto polygons = Helper::getPolygons(HEIGHT, pointsAM8_R[state]);
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
			auto polygons = state >= 0 ? Helper::getPolygons(HEIGHT, pointsAM8_R[state]) : Helper::getPolygons(HEIGHT, pointsAM8_L[-state]);
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
			auto polygons = state >= 0 ? Helper::getPolygons(HEIGHT, pointsAM8_R[state]) : Helper::getPolygons(HEIGHT, pointsAM8_L[-state]);
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
