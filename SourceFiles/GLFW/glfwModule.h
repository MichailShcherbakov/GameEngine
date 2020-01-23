#ifndef _GLFW_MODULE_H_
#define _GLFW_MODULE_H_

#include <Engine/Engine.h>

#define GLFW_EXPOSE_NATIVE_WIN32

#include "glfw3.h"
#include "glfw3native.h"

class GLFWModule : public IEngineModule
{
public:
	GLFWModule();
	~GLFWModule() override;

public:
	// IEngineModule
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Release() override;
	virtual const char* GetModuleName() { return "Engine.GLFW"; }
	virtual void RegisterEngineModule(const IInitializeParams& params)  override;
	virtual void OnEngineEvent(const EEngineEvent& event) override;
	// ~IEngineModule

private:
	GLFWwindow* m_pWindow = nullptr;
};

REGISTER_ENGINE_MODULE(GLFWModule)

#endif // _RENDERER_VULKAN_MODULE_H_