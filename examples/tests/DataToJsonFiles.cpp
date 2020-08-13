#include "catch2/catch.hpp"
#include "Common.h"
#include "CN220DATA.h"
#include <fstream>
#include <iostream>
#include <iomanip>

using json = nlohmann::json;
TEST_CASE("DataToJsonFiles", "[DataToJsonFiles]")
{
	for (int i = 0; i < (int)pointsCN220_L.size(); ++i)
	{
		auto const &data = pointsCN220_L[i];
		auto j = Common::dataToJson(data);

		char path[256] = { 0 };
		snprintf(path, sizeof(path), "state%d.json", i);
		std::ofstream o(std::string(path).data());
		if (!o.is_open())
		{
			return;
		}
		o << std::setw(4) << j;
	}
	for (int i = 0; i < (int)pointsCN220_R.size(); ++i)
	{
		auto const &data = pointsCN220_R[i];
		auto j = Common::dataToJson(data);

		char path[256] = { 0 };
		snprintf(path, sizeof(path), "state%d.json", i + pointsCN220_L.size());
		std::ofstream o(std::string(path).data());
		if (!o.is_open())
		{
			return;
		}
		o << std::setw(4) << j;
	}
}