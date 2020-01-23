#include "StdAfx.h"
#include "Console.h"

#include <iostream>

Console::~Console()
{
	gEngine->pLogSystem->OnLogWrite -= LISTENER(*this, Console::OnLogWrite);
}

void Console::Initialize()
{
	gEngine->pLogSystem->OnLogWrite += LISTENER(*this, Console::OnLogWrite);
}

void Console::OnLogWrite(const std::string& msg, ELogType type)
{
	std::cout << msg << std::endl;
}
