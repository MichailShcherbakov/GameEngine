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
}

void RendererOpenglModule::RegisterEngineModule(const IInitializeParams& params)
{
	gEngine = params.gEngine;
}
