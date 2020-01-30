#ifndef _I_CORE_MODULE_H_
#define _I_CORE_MODULE_H_

#include "EventSystem/Event.h"

struct IInitializeParams;
struct ILoader;

enum class EEngineEvent : uint16_t
{
	EVENT_ENGINE_CORE_INIT_COMPLETE,
	EVENT_ENGINE_GLFW_INIT_COMPLETE,
	EVENT_ENGINE_SURFACE_INIT_COMPLETE,
	EVENT_ENGINE_RENDERER_INIT_COMPLETE,
};

struct ICoreModule
{
	MAKE_EVENT(OnEngineEvent, const EEngineEvent&)


	typedef ICoreModule* (*TModulePoint)();

	virtual ~ICoreModule() {}

	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Release() = 0;
	virtual void Quit() = 0;
	virtual bool IsQuiting() = 0;
	virtual void RunCoreLoop() = 0;
	virtual const char* GetModuleName() = 0;
	virtual std::string GetRootDir() = 0;
	virtual void LoadEngineModules(const IInitializeParams& params) = 0;
	virtual void EngineEvent(const EEngineEvent& event) = 0;
	virtual ILoader* GetMeshLoader() = 0;
};

#endif // _I_CORE_MODULE_H_