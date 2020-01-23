#ifndef _I_CORE_MODULE_H_
#define _I_CORE_MODULE_H_

struct IInitializeParams;

struct ICoreModule
{
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
};

#endif // _I_CORE_MODULE_H_