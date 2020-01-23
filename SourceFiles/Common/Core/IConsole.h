#ifndef _I_CONSOLE_H_
#define _I_CONSOLE_H_

#include <string>

#include "ILogSystem.h"

struct IConsole
{
	virtual ~IConsole() {}

	virtual void Initialize() = 0;
	virtual void OnLogWrite(const std::string& msg, ELogType type) = 0;
};

#endif // _I_CONSOLE_H_