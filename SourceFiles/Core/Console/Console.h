#ifndef _CONSOLE_H_
#define _CONSOLE_H_

#include <Engine/Engine.h>
#include <Core/IConsole.h>

#include "EventSystem/Event.h"

class Console : public IConsole
{
	~Console() override;

	virtual void Initialize() override;
	virtual void OnLogWrite(const std::string& msg, ELogType type) override;
};

#endif // _CONSOLE_H_