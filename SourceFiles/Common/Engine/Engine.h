#ifndef _ENGINE_H_
#define _ENGINE_H_

#include <string>
#include <iostream>

struct ICoreModule;
struct ILogSystem;
struct ISurface;

#ifdef _WIN32
#define USE_PLATFORM_WINDOWS	

#ifdef USE_PLATFORM_WINDOWS
	#include <Windows.h>

	#define DLL_EXPORT __declspec(dllexport)
	#define DLL_IMPORT __declspec(dllimport)
#endif // USE_PLATFORM_WINDOWS

#endif // _WIN32

#ifdef __GNUC__
#define USE_PLATFORM_LINUX

#ifdef USE_PLATFORM_LINUX
	#define DLL_EXPORT __attribute__ ((dllexport))
	#define DLL_IMPORT __attribute__ ((dllimport))
#endif // USE_PLATFORM_LINUX

#endif // __GNUC__

#define ENGINE_API DLL_EXPORT

#include "ICoreModule.h"
#include "IEngineModule.h"

#define REGISTER_ENGINE_MODULE(name) \
extern "C" \
{ \
	DLL_EXPORT IEngineModule* __stdcall CreateEngineModule() \
	{ \
		return new name(); \
	} \
};

#define ENGINE_CORE_MODULE(name) \
extern "C" \
{ \
	DLL_EXPORT ICoreModule* __stdcall CreateEngineCoreModule() \
	{ \
		return new name(); \
	} \
};

typedef HMODULE EngineModule;

static EngineModule LoadModule(const std::string& path)
{
	EngineModule module = nullptr;

#ifdef USE_PLATFORM_WINDOWS
	std::string dllPath = path + ".dll";
	module = LoadLibraryA(dllPath.c_str());
#endif

#ifdef USE_PLATFORM_LINUX
	std::string soPath = "./lib" + path + ".so";
	module = dlopen(soPath.c_str(), RTLD_NOW | RTLD_LOCAL);
#endif

	return module;
}

template <class D>
static D EngineGetProcAddress(EngineModule moule, const std::string& func)
{
#ifdef USE_PLATFORM_WINDOWS
	return reinterpret_cast<D>(GetProcAddress(moule, func.c_str()));
#endif

#ifdef USE_PLATFORM_LINUX
	return reinterpret_cast<D>(dlsym(moule, func));
#endif 
}

static void EngineModuleFree(EngineModule module)
{
#ifdef USE_PLATFORM_WINDOWS
	FreeLibrary(module);
#endif

#ifdef USE_PLATFORM_LINUX
	dlclose(module);
#endif 
}

enum class EMessageBoxType : short
{
	Information,
	Error,
	Warning
};

struct ICoreGlobalVariable
{
	ICoreModule* pCore = nullptr;
	ILogSystem* pLogSystem = nullptr;
	ISurface* pSurface = nullptr;
};

struct IInitializeParams
{
	ICoreGlobalVariable* gEngine;
};

extern ICoreGlobalVariable* gEngine;

#ifndef Message
void EngineMessageBox(const std::string& title, const std::string& msg, EMessageBoxType type);
#endif

#ifndef Message
#define Message(title, msg, type) \
	EngineMessageBox(title, msg, type)
#endif // Message 

#ifndef SAFE_DELETE
#define SAFE_DELETE(p) { if(p) { delete (p); (p)=nullptr; } }
#endif // SAFE_DELETE 

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) { if(p) { delete [] (p); (p)=nullptr; } }
#endif // SAFE_DELETE_ARRAY 

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=nullptr; } }
#endif // SAFE_RELEASE 

#ifndef ASSERT
#define ASSERT(x, y, m) { if(x == y) { Message("FATAL ERROR", m, EMessageBoxType::Error); } }
#endif // ASSERT

#ifndef SAFE_DELETE_POINTERS_ARRAY
#define SAFE_DELETE_POINTERS_ARRAY(array) \
for (auto it : array) \
{ \
	delete it; \
}
#endif // SAFE_DELETE_POINTERS_ARRAY

#endif // _ENGINE_H_