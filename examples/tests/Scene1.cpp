#include "catch2/catch.hpp"
#include "parkassist/gles/fwd.h"
#include "Parser.h"
#include "Window.h"
#include "spdlog/spdlog.h"
#include "Timer.h"

using namespace nb;

Parser g_parser;
int g_currentState = 0;

void prevState()
{
	auto drawingStateCount = g_parser.drawingStatesCount();
	g_currentState = g_currentState - 1;
	if (g_currentState < 0)
		g_currentState = 0;
//	Application::current()->drawContext.renderers() = g_parser.getDrawingState(g_currentState);
//	spdlog::info("state={}", g_currentState);
}

void nextState()
{
	auto drawingStateCount = g_parser.drawingStatesCount();
	g_currentState = g_currentState + 1;
	if (g_currentState >= drawingStateCount)
		g_currentState = drawingStateCount - 1;
//	Application::current()->drawContext.renderers() = g_parser.getDrawingState(g_currentState);
//	spdlog::info("state={}", g_currentState);
}

void updateCurrentState()
{
//	Application::current()->drawContext.renderers() = g_parser.getDrawingState(g_currentState);
}

TEST_CASE("Test Scene1", "[Scene1]")
{
//	Application app;
	Window w(800, 600, "Scene1");
	w.KeyEvent += [](const int &key) {
		switch (key)
		{
		case 262:	prevState();	break;
		case 263:	nextState();	break;
		case 32: {g_parser.enablePointsObjects(!g_parser.isPointsObjectsOn()); updateCurrentState(); }	break;
		default:
			break;
		}
	};

	g_parser.setDir("../etc/states-scene1");
	if (!g_parser.parse())
	{
		return;
	}

	w.resize(g_parser.getContextWidth(), g_parser.getContextHeight());
	
	auto drawingStateCount = g_parser.drawingStatesCount();
	if (drawingStateCount != 0)
	{
	//	app.drawContext.renderers() = g_parser.getDrawingState(0);
	}

	Timer timer;
	timer.setInterval(100);
	timer.Tick += [](const Timer::TickEventArgs &args) 
	{
		nextState();
	};
	//timer.start();

//	app.run(0, nullptr);
}