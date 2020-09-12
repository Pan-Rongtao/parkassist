#include "parkassist/gles/Rectangle.h"

using namespace nb;

Rectangle::Rectangle()
{
	vertexs.resize(4);
	vertexs[0].texCoord = glm::vec2(0.0, 1.0);
	vertexs[1].texCoord = glm::vec2(1.0, 1.0);
	vertexs[2].texCoord = glm::vec2(1.0, 0.0);
	vertexs[3].texCoord = glm::vec2(0.0, 0.0);
	indices = {0, 1, 2, 0, 2, 3};
}

void Rectangle::set(const glm::vec2 & p0, const glm::vec2 & p1, const glm::vec2 & p2, const glm::vec2 & p3)
{
	vertexs[0].position = glm::vec3(p0, 0.0f);
	vertexs[1].position = glm::vec3(p1, 0.0f);
	vertexs[2].position = glm::vec3(p2, 0.0f);
	vertexs[3].position = glm::vec3(p3, 0.0f);
}
