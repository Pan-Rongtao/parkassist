#pragma once
#include <vector>
#include "glm/glm.hpp"
#include "parkassist/gles/fwd.h"
#include "nlohmann/json.hpp"

using json = nlohmann::json;
class Common
{
public:
	static std::vector<glm::vec2> doublesToSide(const std::vector<double> &doubles)
	{
		std::vector<glm::vec2> side(doubles.size() / 2);
		for (size_t i = 0; i != side.size(); ++i)
		{
			side[i] = { doubles[i * 2], doubles[i * 2 + 1] };
		}
		return side;
	}

	static std::shared_ptr<Polygon> getBackground(const std::string &path, int width, int height)
	{
		auto polygonBG = std::make_shared<Polygon>();
		polygonBG->setSide0({ { 0, height },{ 0,0 } });
		polygonBG->setSide1({ { width, height },{ width,0 } });
		polygonBG->setBezierParams(0, 0);
		auto imgBrush = std::make_shared<ImageBrush>(std::make_shared<ImageSource>(path));
		polygonBG->setBrush(imgBrush);
		return polygonBG;
	}

	static std::vector<PolygonPtr> getPolygons(const std::vector<std::vector<double>> &data)
	{
		std::vector<PolygonPtr> polygons;
		for (size_t i = 0; i < data.size() / 3; ++i)
		{
			auto const &side0Data = data[i * 3 + 0];
			auto const &side1Data = data[i * 3 + 1];
			auto const &colorData = data[i * 3 + 2];
			BrushPtr brush;
			if (colorData.size() <= 4)
			{
				Color color{ (uint8_t)colorData[0], (uint8_t)colorData[1], (uint8_t)colorData[2], (uint8_t)colorData[3] };
				brush = std::make_shared<SolidColorBrush>(color);
			}
			else
			{
				std::vector<GradientStop> gradientStops;
				for (size_t k = 0; k < colorData.size() / 5; ++k)
				{
					Color color((uint8_t)colorData[k * 5 + 0], (uint8_t)colorData[k * 5 + 1], (uint8_t)colorData[k * 5 + 2], (uint8_t)colorData[k * 5 + 3]);
					float offset = (float)colorData[k * 5 + 4];
					auto stop = GradientStop{ color, offset };
					gradientStops.push_back(stop);
				}
				brush = std::make_shared<LinearGradientBrush>(gradientStops);
			}
			auto polygon = std::make_shared<Polygon>(doublesToSide(side0Data), doublesToSide(side1Data));
			polygon->setBrush(brush);

			polygons.push_back(polygon);
		}
		return polygons;
	}

	static json dataToJson(const std::vector<std::vector<double>> &data)
	{
		json root;
		for (size_t i = 0; i < data.size() / 3; ++i)
		{
			auto const &side0Data = data[i * 3 + 0];
			auto const &side1Data = data[i * 3 + 1];
			auto const &colorData = data[i * 3 + 2];
			json polygon;
			polygon["Side0"] = side0Data;
			polygon["Side1"] = side1Data;
			polygon["Color"] = colorData;
			auto polygonName = std::string("Polygon") + char(char('a') + i);
			root[polygonName] = polygon;
		}
		return root;
	}

};
