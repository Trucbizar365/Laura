#ifndef LOG_H
#define LOG_H

#include <memory>
#include "spdlog/spdlog.h"

class Log
{
public:
	Log();
	~Log();
	static void Init();

	inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
	inline static std::shared_ptr<spdlog::logger>& GetAppLogger() { return s_AppLogger; }

private:
	static std::shared_ptr<spdlog::logger> s_CoreLogger;
	static std::shared_ptr<spdlog::logger> s_AppLogger;
};



#endif // LOG_H