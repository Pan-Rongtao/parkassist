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

TEST_CASE("Test Scene1", "[Scene1]")
{
	Application app;
	Window w(800, 600, "Scene1");

	Programs::primitive();

	Parser parser;
	parser.setDir("../etc/states-scene1");
	bool b = parser.parse();

	w.resize(parser.getContextWidth(), parser.getContextHeight());
	
	if (b)
	{
		auto const &states = parser.drawingStates();

		Timer timer;
		timer.setInterval(1000);
		timer.Tick += [states](const Timer::TickEventArgs &args) {
			static int i = 0;
			if (i >= states.size())
			{
				i = 0;
			}
			Application::current()->drawContext.renderers() = states[i];
			++i;
		};
		timer.start();


		//makePolygon();
		app.run(0, nullptr);
	}
}