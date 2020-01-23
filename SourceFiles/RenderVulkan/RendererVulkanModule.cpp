#include "StdAfx.h"
#include "RendererVulkanModule.h"

RendererVulkanModule::RendererVulkanModule()
{
}

RendererVulkanModule::~RendererVulkanModule()
{
}

void RendererVulkanModule::Initialize()
{
}

void RendererVulkanModule::Update()
{
}

void RendererVulkanModule::Release()
{
}

void RendererVulkanModule::RegisterEngineModule(const IInitializeParams& params)
{
	gEngine = params.gEngine;
}
