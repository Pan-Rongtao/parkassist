#pragma once
#include <vector>
#include "glm/glm.hpp"
#include "parkassist/gles/Polygon.h"
#include "parkassist/gles/Rectangle.h"
#include "parkassist/gles/Bitmap.h"
#include "parkassist/gles/Scene.h"
#include "nlohmann/json.hpp"

using json = nlohmann::json;
using namespace nb;

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

	static MeshPtr getBackground(const std::string &path, int width, int height)
	{
		auto bg = std::make_shared<Rectangle>();
		bg->set({ 0, height }, { width,height }, { width, 0 }, { 0,0 });

		auto texture = std::make_shared<Texture2D>();
		Bitmap bm(path);
		auto x = Texture::getGlFormatAndType(bm.channels());
		texture->update(bm.data(), bm.width(), bm.height(), x.first, x.second);
		bg->material = std::make_shared<ImageMaterial>(texture);
		return bg;
	}

	static std::vector<PolygonPtr> getPolygons(float height, const std::vector<std::vector<double>> &data)
	{
		std::vector<PolygonPtr> polygons;
		for (size_t i = 0; i < data.size() / 3; ++i)
		{
			auto const &side0Data = data[i * 3 + 0];
			auto const &side1Data = data[i * 3 + 1];
			auto const &colorData = data[i * 3 + 2];
			MaterialPtr material;
			if (colorData.size() <= 4)
			{
				Color color{ (uint8_t)colorData[0], (uint8_t)colorData[1], (uint8_t)colorData[2], (uint8_t)colorData[3] };
				material = std::make_shared<ColorMaterial>(color);
			}
			else
			{
				std::vector<GradientStop> gradientStops;
				for (size_t k = 0; k < colorData.size() / 5; ++k)
				{
					Color color((uint8_t)colorData[k * 5 + 0], (uint8_t)colorData[k * 5 + 1], (uint8_t)colorData[k * 5 + 2], (uint8_t)colorData[k * 5 + 3]);
					float offset = (float)colorData[k * 5 + 4];
					GradientStop stop{ offset, color };
					gradientStops.push_back(stop);
				}
				material = std::make_shared<LinearGrandientMaterial>(height, gradientStops);
			}
			auto polygon = std::make_shared<Polygon>();
			polygon->set(doublesToSide(side0Data), doublesToSide(side1Data));
			polygon->material = material;

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
