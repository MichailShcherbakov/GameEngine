#ifndef _RENDERER_VULKAN_STDAFX_H_
#define _RENDERER_VULKAN_STDAFX_H_

#include <Engine/Engine.h>
#include <LogSystem/LogSystem.h>

#define MODULE_TITILE "[RendererVulkan] "

#define _Log(...) gEngine->pLogSystem->Log(MODULE_TITILE __VA_ARGS__);
#define _LogWarning(...) gEngine->pLogSystem->LogWarning(MODULE_TITILE __VA_ARGS__);
#define _LogError(...) gEngine->pLogSystem->LogError(MODULE_TITILE __VA_ARGS__);
#define _LogSuccess(...) gEngine->pLogSystem->LogSuccess(MODULE_TITILE __VA_ARGS__);

#define FATAL_ERROR(x) { Message("FATAL ERROR", x, EMessageBoxType::Error); }

#endif // _RENDERER_VULKAN_STDAFX_H_