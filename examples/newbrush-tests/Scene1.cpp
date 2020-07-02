#include "newbrush/gles/Def.h"
#include "catch2/catch.hpp"
#include "Application.h"
#include "newbrush/gles/Program.h"
#include "newbrush/gles/Quadrangle.h"
#include "newbrush/gles/RenderObject.h"
#include "newbrush/gles/Polygon.h"
#include "Parser.h"
#include "spdlog/spdlog.h"

using namespace nb;

class Settled
{
public:
	void draw()
	{

	}

private:
	std::array<RenderObject, 7> m_objs;
};

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
	parser.setDir("D:/github/Parkassist/examples/newbrush-tests/states");
	bool b = parser.parse();

	if (b)
	{
		auto const &states = parser.drawingStates();
		if (!states.empty())
		{
			Application::current()->drawContext.renderers() = states[0];
		}
		//makePolygon();
		app.run(0, nullptr);
	}
}