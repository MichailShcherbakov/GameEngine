#ifndef _I_ENGINE_MODULE_H_
#define _I_ENGINE_MODULE_H_

struct IInitializeParams;

struct IEngineModule
{
	typedef IEngineModule* (*TModulePoint)();

	virtual ~IEngineModule() {};

	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Release() = 0;
	virtual const char* GetModuleName() = 0;
	virtual void RegisterEngineModule(const IInitializeParams& params) = 0;
};

#endif // _I_ENGINE_MODULE_H_