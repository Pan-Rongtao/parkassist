#pragma once
#include "parkassist/gles/Mesh.h"

namespace nb {

class NB_API Rectangle : public Mesh
{
public:
	Rectangle();

	void set(const glm::vec2 &p0, const glm::vec2 &p1, const glm::vec2 &p2, const glm::vec2 &p3);
};

using RectanglePtr = std::shared_ptr<Rectangle>;
}