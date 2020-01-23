#ifndef _RENDERER_OPENGL_MODULE_H_
#define _RENDERER_OPENGL_MODULE_H_

#include "Engine/Engine.h"

class RendererOpenglModule : public IEngineModule
{
public:
	RendererOpenglModule();
	~RendererOpenglModule() override;

public:
	// IEngineModule
	virtual void Initialize()  override;
	virtual void Update()  override;
	virtual void Release() override;
	virtual const char* GetModuleName() { return "Engine.RenderOpengl"; }
	virtual void RegisterEngineModule(const IInitializeParams& params)  override;
	virtual void OnEngineEvent(const EEngineEvent& event) override;
	// ~IEngineModule
};

REGISTER_ENGINE_MODULE(RendererOpenglModule)

#endif // _RENDERER_OPENGL_MODULE_H_