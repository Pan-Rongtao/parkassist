#include "catch2/catch.hpp"
#include "Window.h"
#include "Common.h"
#include "parkassist/gles/fwd.h"
#include "Parser.h"
#include <fstream>

using namespace nb;

#define PROJECT_NAME	"ParserDataScene"
#define PROJECT_CFG		"../etc/project.json"
#define WIDTH 1280
#define HEIGHT 800

Parser g_parser;
TEST_CASE(PROJECT_NAME, std::string("[") + PROJECT_NAME + "]")
{
	std::ifstream stream(PROJECT_CFG);
	if (!stream)
	{
		printf("%s not found.\n", PROJECT_CFG);
		return;
	}

	json obj;
	try {
		stream >> obj;
	}
	catch (std::exception &e)
	{
		stream.close();
		printf("parse error=%s.\n", e.what());
		return;
	}

	json j;
	try {
		j = obj.at("Project");
	}
	catch (...) { printf("can't find field [Project] as int type in file [%s]\n", PROJECT_CFG); return; }
	auto projectName = j.get<std::string>();
	auto bkgPicPath = std::string("../etc/") + projectName + "/" + "BKG.bmp";

	Window w(WIDTH, HEIGHT, PROJECT_NAME + std::string(":") + projectName);
	g_parser.setDir(std::string("../etc/") + projectName);
	if (!g_parser.parse())
	{
		return;
	}
	w.resize(g_parser.getContextWidth(), g_parser.getContextHeight());
	std::shared_ptr<Scene> sc = std::make_shared<Scene>(g_parser.getContextWidth(), g_parser.getContextHeight());

	w.ResizeEvent += [&w, &sc](const Window::Size &sz)
	{
		sc->doRender();
		w.swapBuffers();
	};

	int state = 0;
	auto polygonBG = Common::getBackground(bkgPicPath, WIDTH, HEIGHT);
	sc->add(polygonBG);
	auto polygons = g_parser.getDrawingState(state);
	for (auto const p : polygons)
	{
		sc->add(p);
	}
	sc->doRender();
	w.swapBuffers();

	w.KeyEvent += [&w, &sc, &state, polygonBG](const int &key)
	{
		auto index = 0;
		int halfCount = (int)std::ceil(g_parser.drawingStatesCount() / 2.0);
		switch (key)
		{
		case GLFW_KEY_LEFT:
		{
			if (state - 1 <= -halfCount)
			{
				return;
			}

			--state;

			index = state < 0 ? -(state - halfCount + 1) : state;
			auto polygons = g_parser.getDrawingState(index);
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
			if (state + 1 >= halfCount)
			{
				return;
			}

			++state;
			index = state < 0 ? -(state - halfCount + 1) : state;
			auto polygons = g_parser.getDrawingState(index);
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
		printf("state=%d, index=%d\n", state, index);
	};


	while (true)
	{
		Window::pollEvents();
	}
}