#ifndef _RENDERER_VULKAN_MODULE_H_
#define _RENDERER_VULKAN_MODULE_H_

#include "Engine/Engine.h"

class RendererVulkanModule : public IEngineModule
{
public:
	RendererVulkanModule();
	~RendererVulkanModule() override;

public:
	// IEngineModule
	virtual void Initialize()  override;
	virtual void Update()  override;
	virtual void Release() override;
	virtual const char* GetModuleName() { return "Engine.RenderVulkan"; }
	virtual void RegisterEngineModule(const IInitializeParams& params)  override;
	// ~IEngineModule
};

REGISTER_ENGINE_MODULE(RendererVulkanModule)

#endif // _RENDERER_VULKAN_MODULE_H_