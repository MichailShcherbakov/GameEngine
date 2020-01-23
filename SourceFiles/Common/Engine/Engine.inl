#ifndef _ENGINE_INL_H_
#define _ENGINE_INL_H_

#include "Engine.h"

ICoreGlobalVariable* gEngine = nullptr;

#ifdef Message
void EngineMessageBox(const std::string& title, const std::string& msg, EMessageBoxType type)
{
#ifdef USE_PLATFORM_WINDOWS
	if (type == EMessageBoxType::Information)
		MessageBoxA(NULL, msg.c_str(), title.c_str(), MB_OK | MB_ICONERROR);
	else if (type == EMessageBoxType::Error)
	{
		MessageBoxA(NULL, msg.c_str(), title.c_str(), MB_OK | MB_ICONERROR);
		throw std::runtime_error(msg);
	}
	else if (type == EMessageBoxType::Warning)
		MessageBoxA(NULL, msg.c_str(), title.c_str(), MB_OK | MB_ICONERROR);

#endif

#ifdef USE_PLATFORM_LINUX
	if (type == eBT_Information)
		system(string("zenity --info --text=\"" + msg + "\" --title=\"" + title + "\"").c_str());
	else if (type == eBT_Error)
	{
		system(string("zenity --error --text=\"" + msg + "\" --title=\"" + title + "\"").c_str());
		throw std::runtime_error(msg);
	}
	else
		system(string("zenity --warning --text=\"" + msg + "\" --title=\"" + title + "\"").c_str());
#endif
}
#endif // Message

#endif // _ENGINE_INL_H_