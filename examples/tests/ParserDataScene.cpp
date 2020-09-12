#include "catch2/catch.hpp"
#include "Window.h"
#include "Common.h"
#include "Parser.h"
#include <fstream>
#include "Timer.h"

using namespace nb;

std::string getCfgProjectName()
{
#define PROJECT_CFG		"../etc/project.json"
	std::ifstream stream(PROJECT_CFG);
	if (!stream)
	{
		printf("%s not found.\n", PROJECT_CFG);
		return "";
	}

	json obj;
	try {
		stream >> obj;
	}
	catch (std::exception &e)
	{
		stream.close();
		printf("parse error=%s.\n", e.what());
		return "";
	}

	json j;
	try {
		j = obj.at("Project");
	}
	catch (...) { printf("can't find field [Project] as int type in file [%s]\n", PROJECT_CFG); return ""; }
	return j.get<std::string>();
}

int state = -33;
enum class Direction
{
	prev,
	next,
};
void gotoState(ScenePtr sc, MeshPtr bkg, const Parser &parser, Direction d)
{
	int halfCount = (int)std::ceil(parser.drawingStatesCount() / 2.0);
	if ((d == Direction::prev && state - 1 > -halfCount) || (d == Direction::next && state + 1 < halfCount))
	{
		state = d == Direction::next ? state + 1 : state - 1;
		auto index = state < 0 ? -(state - halfCount + 1) : state;
		auto polygons = parser.getDrawingState(index);
		sc->clear();
		sc->add(bkg);
		for (auto const p : polygons)
		{
			sc->add(p);
		}
		printf("state=%d, index=%d\n", state, index);
	}
	sc->doRender();
}

TEST_CASE("ParserDataScene", "[ParserDataScene]")
{
	Window w(800, 600, "parse data from json");

	Parser parser;
	auto projectName = getCfgProjectName();
	parser.setDir(std::string("../etc/") + projectName);
	if (!parser.parse())	return;

	auto width = parser.getContextWidth();
	auto height = parser.getContextHeight();
	auto bkgPicPath = std::string("../etc/") + projectName + "/" + "BKG.bmp";
	auto bkg = Common::getBackground(bkgPicPath, width, height);

	auto sc = std::make_shared<Scene>(width, height);

	w.resize(width, height);
	w.ResizeEvent += [&w, &sc](const Window::Size &sz) { sc->doRender(); w.swapBuffers(); };
	w.KeyEvent += [&w, &sc, bkg, &parser](const int &key)
	{
		switch (key)
		{
		case 263: gotoState(sc, bkg, parser, Direction::prev);	break;
		case 262:gotoState(sc, bkg, parser, Direction::next);	break;
		case 32:{ sc->enableBorder(!sc->isBorderEnable()); sc->doRender(); break; }
		default:	break;
		}
		w.swapBuffers();
	};

	gotoState(sc, bkg, parser, Direction::next);
	w.swapBuffers();

	Timer timer;
	timer.setInterval(10);
	timer.Tick += [sc, bkg, parser, &w](const Timer::TickEventArgs &args)
	{
		gotoState(sc, bkg, parser, Direction::next);
		w.swapBuffers();
	};
//	timer.start();

	while (true)
	{
		Window::pollEvents();
		Timer::driveInLoop();
	}
}
