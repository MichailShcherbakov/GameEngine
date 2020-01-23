#ifndef _CORE_STDAFX_H_
#define _CORE_STDAFX_H_

#include <Engine/Engine.h>
#include "LogSystem/LogSystem.h"

#define MODULE_TITILE "[Core] "

#define _Log(...) gEngine->pLogSystem->Log(MODULE_TITILE __VA_ARGS__);
#define _LogWarning(...) gEngine->pLogSystem->LogWarning(MODULE_TITILE __VA_ARGS__);
#define _LogError(...) gEngine->pLogSystem->LogError(MODULE_TITILE __VA_ARGS__);
#define _LogSuccess(...) gEngine->pLogSystem->LogSuccess(MODULE_TITILE __VA_ARGS__);

#define FATAL_ERROR(x) { RadMessageBox("FATAL ERROR", x, eBT_Error); }

#endif // _CORE_STDAFX_H_