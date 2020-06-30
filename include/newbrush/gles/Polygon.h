#pragma once
#include "Model.h"

namespace nb {

class NB_API Polygon : public Model
{
public:
	Polygon() = default;

	void setPoint(const std::vector<glm::vec2> &side0, const std::vector<glm::vec2> &side1);

private:
	std::vector<glm::vec2> doBezier(const std::vector<glm::vec2> &inputs);
};

}