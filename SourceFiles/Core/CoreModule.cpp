#include "StdAfx.h"
#include "CoreModule.h"

#include "LogSystem/LogSystem.h"
#include "Console/Console.h"

#include <boost/filesystem.hpp>

CoreModule::CoreModule()
{
}

CoreModule::~CoreModule()
{
	SAFE_DELETE(m_console);
}

void CoreModule::Initialize()
{
	gEngine = new ICoreGlobalVariable();

	gEngine->pCore = this;
	gEngine->pLogSystem = new LogSystem();
	gEngine->pLogSystem->Initialize();

	m_console = new Console();
	m_console->Initialize();

	_Log("Initializing Engine...");

	m_pInitializeParams.gEngine = gEngine;

	LoadEngineModules(m_pInitializeParams);
}

void CoreModule::Update()
{
}

void CoreModule::Release()
{
}

void CoreModule::Quit()
{
	m_bIsEngineLoop = false;
}

bool CoreModule::IsQuiting()
{
	return m_bIsEngineLoop;
}

void CoreModule::RunCoreLoop()
{
	m_bIsEngineLoop = true;

	while (m_bIsEngineLoop)
	{
		
	}
}

std::string CoreModule::GetRootDir()
{
	return boost::filesystem::absolute("")
		.parent_path()
		.parent_path()
		.parent_path()
		.string()
		.c_str();
}

void CoreModule::LoadEngineModules(const IInitializeParams& params)
{
	for (auto& lib : libs)
	{
		EngineModule module = LoadModule(lib);

		ASSERT(module, nullptr, "The loading Library is not valid...");

		auto obj = EngineGetProcAddress<IEngineModule::TModulePoint>(module, "CreateEngineModule");

		ASSERT(obj, nullptr, "Couldn't get function \"CreateEngineModule\"...");

		auto objectDll = obj();

		ASSERT(objectDll, nullptr, "Couldn't create library object...");

		objectDll->RegisterEngineModule(params);

		_LogSuccess("[%s] Library has been loaded.", objectDll->GetModuleName());

		m_moduels.push_back(new Module(module, objectDll));
	}
}

void CoreModule::ReleaseLoadedEngineModules()
{
	for (auto module : m_moduels)
	{
		_Log("[%s] Releasing the library...", module->object->GetModuleName());
		module->object->Release();
		SAFE_DELETE(module);
	}
}
