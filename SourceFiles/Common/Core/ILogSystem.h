#ifndef _I_LOG_SYSTEM_H__
#define _I_LOG_SYSTEM_H__

#define PRINTF_PARAMS(...)
#define SCANF_PARAMS(...)

#include "EventSystem/Event.h"

enum class ELogType : short
{
	Log,
	LogError,
	LogWarning,
	LogSuccess,
	FatalError
};

struct ILogSystem
{
	MAKE_EVENT(OnLogWrite, const std::string&, ELogType);

	virtual ~ILogSystem() {}

	virtual void Initialize() = 0;
	virtual void Log(const char* msg, ...) PRINTF_PARAMS(2, 3) = 0;
	virtual void LogError(const char* msg, ...) PRINTF_PARAMS(2, 3) = 0;
	virtual void LogWarning(const char* msg, ...) PRINTF_PARAMS(2, 3) = 0;
	virtual void LogSuccess(const char* msg, ...) PRINTF_PARAMS(2, 3) = 0;
	virtual void FatalError(const char* msg) = 0;
};

#endif
