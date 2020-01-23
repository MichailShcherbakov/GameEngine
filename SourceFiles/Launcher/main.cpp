#define DEBUG

#ifdef RELEASE

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int mCmdShow)
{
	return 0;
}

#endif

#ifdef DEBUG

#include "Engine/Engine.h"

int main(int argc, char** argv)
{
	EngineModule module = LoadModule("Engine.Core");
	ICoreModule* pEngineCore = EngineGetProcAddress<ICoreModule::TModulePoint>(module, "CreateEngineCoreModule")();
	
	pEngineCore->Initialize();
	pEngineCore->RunCoreLoop();
	pEngineCore->Release();
}

#endif