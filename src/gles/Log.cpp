#include "parkassist/gles/Log.h"
#include "spdlog/sinks/stdout_color_sinks.h"

using namespace nb;
//∏Ò Ω£∫https://github.com/gabime/spdlog/wiki/3.-Custom-formatting

std::shared_ptr<spdlog::logger> Log::nbLogger()
{
	static std::shared_ptr<spdlog::logger> g_nbLogger;
	if (!g_nbLogger)
	{
		auto sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
		g_nbLogger = std::make_shared<spdlog::logger>("nb", sink);
		//g_nbLogger->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%n] [%^%l%$] %v");
	}
	return g_nbLogger;
}
