#pragma once
#include "parkassist/Def.h"
#include "spdlog/spdlog.h"

namespace nb {

class NB_API Log
{
public:
	void setLevel(spdlog::level::level_enum level)
	{
		nbLogger()->set_level(level);
	}

	template<typename FormatString, typename... Args>
	static void trace(const FormatString &fmt, const Args &... args)
	{
		nbLogger()->trace(fmt, args...);
	}

	template<typename FormatString, typename... Args>
	static void debug(const FormatString &fmt, const Args &... args)
	{
		nbLogger()->debug(fmt, args...);
	}

	template<typename FormatString, typename... Args>
	static void info(const FormatString &fmt, const Args &... args)
	{
		nbLogger()->info(fmt, args...);
	}

	template<typename FormatString, typename... Args>
	static void warn(const FormatString &fmt, const Args &... args)
	{
		nbLogger()->warn(fmt, args...);
	}

	template<typename FormatString, typename... Args>
	static void error(const FormatString &fmt, const Args &... args)
	{
		nbLogger()->error(fmt, args...);
	}

	template<typename FormatString, typename... Args>
	static void critical(const FormatString &fmt, const Args &... args)
	{
		nbLogger()->critical(fmt, args...);
	}

private:
	static std::shared_ptr<spdlog::logger> nbLogger();
};


}