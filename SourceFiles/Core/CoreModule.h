#ifndef _CORE_MODULE_H_
#define _CORE_MODULE_H_

#include <Engine/Engine.h>
#include <Core/IConsole.h>

#include <Renderer/RendererVulkan.h>

class CoreModule : public ICoreModule
{
	struct Module
	{
		EngineModule module;
		IEngineModule* object;

		Module(const EngineModule& module, IEngineModule* object)
		{
			this->module = module;
			this->object = object;
		}

		~Module()
		{
			if (object)
				delete object;

			EngineModuleFree(module);
			object = nullptr;
		}
	};

	using Modules = std::vector<Module*>;

public:
	CoreModule();
	~CoreModule() override;

public:
	// IUnknownModule
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Release() override;
	virtual const char* GetModuleName() { return "Engine.Core"; };
	virtual void Quit() override;
	virtual bool IsQuiting() override;
	virtual void RunCoreLoop() override;
	virtual std::string GetRootDir() override;
	virtual void LoadEngineModules(const IInitializeParams& params) override;
	virtual void EngineEvent(const EEngineEvent& event) override;
	virtual ILoader* GetMeshLoader() override;
	// ~IUnknownModule

public:
	void ReleaseLoadedEngineModules();

private:
	Modules m_modules;
	IConsole* m_console;
	ILoader* m_meshLoader;
	IInitializeParams m_pInitializeParams;

private:
	std::vector<std::string> libs = 
	{
		"Engine.GLFW",
		"Engine.RenderVulkan",
		"Engine.RenderOpengl",
	};

private:
	bool m_bIsEngineLoop = false;
};

ENGINE_CORE_MODULE(CoreModule)

#endif // _CORE_MODULE_H_