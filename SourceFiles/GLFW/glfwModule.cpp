#include "StdAfx.h"
#include "glfwModule.h"

#include <Graphics/Surface/ISurface.h>

GLFWModule::GLFWModule()
{
}

GLFWModule::~GLFWModule()
{
	if (m_pWindow)
		glfwTerminate();
		
	gEngine->pCore->OnEngineEvent -= LISTENER(*this, GLFWModule::OnEngineEvent);
}

void GLFWModule::Initialize()
{
	if (!glfwInit())
		FATAL_ERROR("GLFW was not initialized");

	m_pWindow = glfwCreateWindow(1280, 920, "RenderVulkan", NULL, NULL);

	if (!m_pWindow)
	{
		FATAL_ERROR("Can't create glfw window");
		glfwTerminate();
	}

	glfwMakeContextCurrent(m_pWindow);

	gEngine->pSurface = new ISurface(glfwGetWin32Window(m_pWindow));

	gEngine->pCore->EngineEvent(EEngineEvent::EVENT_ENGINE_SURFACE_INIT_COMPLETE);
}

void GLFWModule::Update()
{
	if (!glfwWindowShouldClose(m_pWindow))
	{
		glfwPollEvents();
	}
	else
	{
		gEngine->pCore->Quit();
	}
}

void GLFWModule::Release()
{
	_Log("Releasing the module...");

	this->~GLFWModule();
}

void GLFWModule::RegisterEngineModule(const IInitializeParams& params)
{
	gEngine = params.gEngine;

	gEngine->pCore->OnEngineEvent += LISTENER(*this, GLFWModule::OnEngineEvent);

	_LogSuccess("Module has been loaded.");
}

void GLFWModule::OnEngineEvent(const EEngineEvent& event)
{
	switch (event)
	{
	case EEngineEvent::EVENT_ENGINE_CORE_INIT_COMPLETE:
		Initialize();
		break;
	}
}
