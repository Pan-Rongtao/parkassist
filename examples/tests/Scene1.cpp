#include "parkassist/gles/Def.h"
#include "catch2/catch.hpp"
#include "Application.h"
#include "parkassist/gles/Program.h"
#include "parkassist/gles/RenderObject.h"
#include "parkassist/gles/Polygon.h"
#include "Parser.h"
#include "spdlog/spdlog.h"
#include "Timer.h"

using namespace nb;

void makePolygon()
{
	std::vector<glm::vec2> side0 = 
	{
		glm::vec2(86, 363), glm::vec2(127, 308), glm::vec2(173, 259), 
	};
	std::vector<glm::vec2> side1 =
	{
		glm::vec2(89, 366), glm::vec2(130, 312),glm::vec2(176, 262),
	};
	auto polygon = std::make_shared<nb::Polygon>();
	polygon->setPoint(side0, side1);
	auto renderObj = std::make_shared<RenderObject>(polygon, Programs::primitive());
	renderObj->storeUniform("color", glm::vec4(1.0, 0.0, 0.0, 1.0));
	renderObj->model()->unifyColor(glm::vec4(1.0, 0.0, 0.0, 1.0));
	renderObj->model()->meshes[0].vertexs[0].color = glm::vec4(0.0, 0.0, 1.0, 1.0);
	renderObj->model()->meshes[0].vertexs[1].color = glm::vec4(0.0, 0.0, 1.0, 1.0);
	Application::current()->drawContext.queue(renderObj);
}

Parser g_parser;
int g_currentState = 0;

void prevState()
{
	auto drawingStateCount = g_parser.drawingStatesCount();
	g_currentState = g_currentState - 1;
	if (g_currentState < 0)
		g_currentState = 0;
	Application::current()->drawContext.renderers() = g_parser.getDrawingState(g_currentState);
//	spdlog::info("state={}", g_currentState);
}

void nextState()
{
	auto drawingStateCount = g_parser.drawingStatesCount();
	g_currentState = g_currentState + 1;
	if (g_currentState >= drawingStateCount)
		g_currentState = drawingStateCount - 1;
	Application::current()->drawContext.renderers() = g_parser.getDrawingState(g_currentState);
//	spdlog::info("state={}", g_currentState);
}

void updateCurrentState()
{
	Application::current()->drawContext.renderers() = g_parser.getDrawingState(g_currentState);
}

TEST_CASE("Test Scene1", "[Scene1]")
{
	Application app;
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

	Programs::primitive();

	g_parser.setDir("../etc/states-scene1");
	bool b = g_parser.parse();

	w.resize(g_parser.getContextWidth(), g_parser.getContextHeight());
	
	if (b)
	{
		auto drawingStateCount = g_parser.drawingStatesCount();
		if (drawingStateCount != 0)
		{
			Application::current()->drawContext.renderers() = g_parser.getDrawingState(0);
		}

		Timer timer;
		timer.setInterval(100);
		timer.Tick += [](const Timer::TickEventArgs &args) {
			nextState();
		};
		//timer.start();

		app.run(0, nullptr);
	}
}