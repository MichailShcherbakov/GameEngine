#ifndef _LOG_SYSTEM_H_
#define _LOG_SYSTEM_H_

#include <Engine/Engine.h>
#include <Core/ILogSystem.h>

#include "EventSystem/Event.h"

#include <vector>

struct SystemInfo
{
	std::string m_ProcessorName;
	std::string m_ProcessorCores;
	std::string m_ProcessorArchitecture;
	std::string m_OS_NamePC;
	std::string m_OS_NameUser;
	std::string m_OS_Version;
	std::string m_OS_BuildVersion;

	std::vector <std::wstring> m_VideoDrivesName;
	std::vector <std::string> m_VideoDrivesMemory;

	std::string m_Ram;
	std::string m_ScreenResolution;
};

class CWriteToLogFile
{
public:
	CWriteToLogFile();
	~CWriteToLogFile();

	void OnLogWrite(const std::string& msg, ELogType type);

private:
	void GetTime();
	void GetSystemInfo();
	void WriteSystemInfoToFile();

private:
	tm* m_pTm;
	SystemInfo* m_pSystemInfo;
	std::string m_currentTime;
	std::string m_currentDate;
	std::string m_path;
};


class LogSystem : public ILogSystem
{
public:
	LogSystem();
	~LogSystem() override;

public:
	// ILogSystem
	virtual void Initialize() override;
	virtual void Log(const char* msg, ...) PRINTF_PARAMS(2, 3) override;
	virtual void LogError(const char* msg, ...) PRINTF_PARAMS(2, 3) override;
	virtual void LogWarning(const char* msg, ...) PRINTF_PARAMS(2, 3) override;
	virtual void LogSuccess(const char* msg, ...) PRINTF_PARAMS(2, 3) override;
	virtual void FatalError(const char* msg) override;
	// ILogSystem

protected:
	void WriteConsoleLog(std::string msg, va_list args, ELogType logType);

public:
	void SetLogFileName(const char* title) { m_logFileName = title; }

private:
	std::vector<std::string> m_logBuffer;
	CWriteToLogFile m_writeLogToFile;
	std::string m_logFileName;
};


#endif // _LOG_SYSTEM_H_