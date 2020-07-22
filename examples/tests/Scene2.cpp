#include "catch2/catch.hpp"
#include "Window.h"
#include "Converter.h"
#include "parkassist/gles/fwd.h"

using namespace nb;

TEST_CASE("Test Scene2", "[Scene2]")
{
	Window w(800, 600, "Scene2");
	w.ResizeEvent += [&w](const Window::Size &sz)
	{
		w.swapBuffers();
	};
	w.resize(600, 540);

	w.KeyEvent += [](const int &key) 
	{
		switch (key)
		{
		case 262:	break;
		case 263:	break;
		case 32: 	break;
		default:	break;
		}
	};

	while (true)
	{
		Window::pollEvents();
	}
}
