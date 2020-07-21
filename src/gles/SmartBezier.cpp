#include "parkassist/gles/SmartBezier.h"
#include "bezier.h"

using namespace nb;

#define MAKE_BEZIER(N, input, out) \
do{\
	std::vector<Bezier::Point> bzInput;\
	for (size_t i = 0; i != input.size(); ++i)\
	{\
		bzInput.push_back({ input[i].x, input[i].y });\
	}\
	Bezier::Bezier<N> bz(bzInput);\
	float step = 1.0 / m_sampleCount;\
	for (float i = 0.0f; i <= 1.000001; i += step)\
	{\
		Bezier::Point p = bz.valueAt(i);\
		out.push_back({ p.x, p.y });\
	}\
} while (0)

SmartBezier::SmartBezier(int maxControlPointsCount, int sampleCount)
	: m_maxControlPointsCount(maxControlPointsCount)
	, m_sampleCount(sampleCount)
{
}

std::vector<glm::vec2> SmartBezier::doBezier(const std::vector<glm::vec2> &inputs)
{
	std::vector<glm::vec2> ret;
	for (size_t i = 0; i + 1 < inputs.size();)
	{
		std::vector<glm::vec2> segOut;
		int controlPointCount = 0;
		if (i + m_maxControlPointsCount >= inputs.size())
		{
			controlPointCount = inputs.size() - i;
		}
		else
		{
			controlPointCount = m_maxControlPointsCount;
		}
		std::vector<glm::vec2> segInput(inputs.begin() + i, inputs.begin() + i + controlPointCount);
		auto step = controlPointCount - 1;
		switch (step)
		{
		case 30:MAKE_BEZIER(30, segInput, segOut);	break;
		case 29:MAKE_BEZIER(29, segInput, segOut);	break;
		case 28:MAKE_BEZIER(28, segInput, segOut);	break;
		case 27:MAKE_BEZIER(27, segInput, segOut);	break;
		case 26:MAKE_BEZIER(26, segInput, segOut);	break;
		case 25:MAKE_BEZIER(25, segInput, segOut);	break;
		case 24:MAKE_BEZIER(24, segInput, segOut);	break;
		case 23:MAKE_BEZIER(23, segInput, segOut);	break;
		case 22:MAKE_BEZIER(22, segInput, segOut);	break;
		case 21:MAKE_BEZIER(21, segInput, segOut);	break;
		case 20:MAKE_BEZIER(20, segInput, segOut);	break;
		case 19:MAKE_BEZIER(19, segInput, segOut);	break;
		case 18:MAKE_BEZIER(18, segInput, segOut);	break;
		case 17:MAKE_BEZIER(17, segInput, segOut);	break;
		case 16:MAKE_BEZIER(16, segInput, segOut);	break;
		case 15:MAKE_BEZIER(15, segInput, segOut);	break;
		case 14:MAKE_BEZIER(14, segInput, segOut);	break;
		case 13:MAKE_BEZIER(13, segInput, segOut);	break;
		case 12:MAKE_BEZIER(12, segInput, segOut);	break;
		case 11:MAKE_BEZIER(11, segInput, segOut);	break;
		case 10:MAKE_BEZIER(10, segInput, segOut);	break;
		case 9:	MAKE_BEZIER(9, segInput, segOut);	break;
		case 8:	MAKE_BEZIER(8, segInput, segOut);	break;
		case 7:	MAKE_BEZIER(7, segInput, segOut);	break;
		case 6:	MAKE_BEZIER(6, segInput, segOut);	break;
		case 5:	MAKE_BEZIER(5, segInput, segOut);	break;
		case 4:	MAKE_BEZIER(4, segInput, segOut);	break;
		case 3:	MAKE_BEZIER(3, segInput, segOut);	break;
		case 2:	MAKE_BEZIER(2, segInput, segOut);	break;
		case 1:	MAKE_BEZIER(1, segInput, segOut);	break;
		default: nbThrowException(std::runtime_error, "unsurport step[%d]", step); break;
		}

		ret.insert(ret.end(), segOut.begin(), segOut.end());
		i += step;
	}
	return ret;
}
