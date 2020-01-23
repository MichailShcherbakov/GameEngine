#include "StdAfx.h"
#include "RendererOpenglModule.h"

RendererOpenglModule::RendererOpenglModule()
{
}

RendererOpenglModule::~RendererOpenglModule()
{
}

void RendererOpenglModule::Initialize()
{
}

void RendererOpenglModule::Update()
{
}

void RendererOpenglModule::Release()
{
	_Log("Releasing the module...");

	this->~RendererOpenglModule();
}

void RendererOpenglModule::RegisterEngineModule(const IInitializeParams& params)
{
	gEngine = params.gEngine;

	_LogSuccess("Module has been loaded.");
}

void RendererOpenglModule::OnEngineEvent(const EEngineEvent& event)
{
}
