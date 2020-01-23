#include "StdAfx.h"
#include "LogSystem.h"
#include <EngineInfo.h>

#include <fstream>
#include <time.h>
#include <dxgi.h>
#include <intrin.h>
#include <time.h>
#include <dxgi.h>
#include <Windows.h>

#include <boost/filesystem.hpp>
#include <rlutil.h>

LogSystem::LogSystem()
{
}

LogSystem::~LogSystem()
{
}

void LogSystem::Initialize()
{
	gEngine->pLogSystem->OnLogWrite += LISTENER(m_writeLogToFile, CWriteToLogFile::OnLogWrite);
}

void LogSystem::Log(const char* msg, ...)
{
	va_list args;
	va_start(args, msg);
	WriteConsoleLog(msg, args, ELogType::Log);
	va_end(args);
}

void LogSystem::LogError(const char* msg, ...)
{
	va_list args;
	va_start(args, msg);
	WriteConsoleLog(msg, args, ELogType::LogError);
	va_end(args);
}

void LogSystem::LogWarning(const char* msg, ...)
{
	va_list args;
	va_start(args, msg);
	WriteConsoleLog(msg, args, ELogType::LogWarning);
	va_end(args);
}

void LogSystem::LogSuccess(const char* msg, ...)
{
	va_list args;
	va_start(args, msg);
	WriteConsoleLog(msg, args, ELogType::LogSuccess);
	va_end(args);
}

void LogSystem::FatalError(const char* msg)
{
	WriteConsoleLog(msg, NULL, ELogType::FatalError);
	Message("FATAL ERROR: ", msg, EMessageBoxType::Error);
	throw std::runtime_error(msg);
}

void LogSystem::WriteConsoleLog(std::string msg, va_list args, ELogType logType)
{
	char* buffer;

	for (int i = 0; i < msg.size(); i++)
	{
		if ((char)msg[i] == '%')
		{
			i++;
			switch ((char)msg[i])
			{
			case 's':
			{
				buffer = va_arg(args, char*);

				if (buffer != nullptr)
				{
					msg.replace(i - 1, 2, buffer);
					i = i - 2;
					i = i + (int)std::string(buffer).size();
				}
				else
				{
					msg.replace(i - 1, 2, "");
				}
				break;
			}
			case 'u':
			{
				msg.replace(i - 1, 2, std::to_string(va_arg(args, int)));
				break;
			}
			case 'f':
			{
				float r = (float)va_arg(args, double);
				msg.replace(i - 1, 2, std::to_string(r));
				break;
			}
			default:
			{
				msg.replace(i - 1, 2, "");
				break;
			}
			}
		}
	}

	emit m_OnLogWrite(msg, logType);
}

CWriteToLogFile::CWriteToLogFile()
{
	m_pTm = new tm;
	m_pSystemInfo = new SystemInfo;

	time_t m_mcTime = time(NULL);
	localtime_s(m_pTm, &m_mcTime);

	m_currentTime = "";
	m_currentTime += ((m_pTm->tm_hour < 10) ? '0' + std::to_string(m_pTm->tm_hour) : std::to_string(m_pTm->tm_hour)) + '.';
	m_currentTime += ((m_pTm->tm_min < 10) ? '0' + std::to_string(m_pTm->tm_min) : std::to_string(m_pTm->tm_min)) + '.';
	m_currentTime += (m_pTm->tm_sec < 10) ? '0' + std::to_string(m_pTm->tm_sec) : std::to_string(m_pTm->tm_sec);

	m_currentDate = "";
	m_currentDate += std::to_string(m_pTm->tm_mday) + ' ';
	switch (m_pTm->tm_mon)
	{
	case 0: m_currentDate += "Jan "; break;
	case 1: m_currentDate += "Feb "; break;
	case 2: m_currentDate += "Mar "; break;
	case 3: m_currentDate += "Apr "; break;
	case 4: m_currentDate += "May "; break;
	case 5: m_currentDate += "Jun "; break;
	case 6: m_currentDate += "Jul "; break;
	case 7: m_currentDate += "Aug "; break;
	case 8: m_currentDate += "Sep "; break;
	case 9: m_currentDate += "Oct "; break;
	case 10: m_currentDate += "Nov "; break;
	case 11: m_currentDate += "Dec "; break;
	}
	m_currentDate += std::to_string(m_pTm->tm_year % 100);


#if defined(USE_PLATFORM_WINDOWS)
	
	boost::filesystem::create_directory(gEngine->pCore->GetRootDir() + "\\Logs");
#endif // !USE_PLATFORM_WINDOWS

	// GameLog - Game Build(...) 13 Dec 18 ( 14:50:10 )
	m_path = std::string(gEngine->pCore->GetRootDir()) + "\\Logs\\Engine Build(" + std::to_string(ENGINE_VERSION_BUILD) + ") " + m_currentDate + " (" + m_currentTime + ").log";

	GetSystemInfo();
	WriteSystemInfoToFile();
}

CWriteToLogFile::~CWriteToLogFile()
{
	gEngine->pLogSystem->OnLogWrite -= LISTENER(*this, CWriteToLogFile::OnLogWrite);
}

void CWriteToLogFile::OnLogWrite(const std::string& msg, ELogType type)
{
	std::string sTypeLog;
	switch (type)
	{
	case ELogType::LogError:
	{
		sTypeLog = "[Error] ";
		break;
	}
	case ELogType::LogWarning:
	{
		sTypeLog = "[Warning] ";
		break;
	}
	case ELogType::LogSuccess:
	{
		sTypeLog = "[Success] ";
		break;
	}
	case ELogType::FatalError:
	{
		sTypeLog = "[FatalError] ";
		break;
	}
	default:
	{
		sTypeLog = "";
		break;
	}
	}

	GetTime();

	std::string sEndLog = "[" + m_currentTime + "] " + sTypeLog + std::string(msg);

	std::fstream m_LogFile(m_path, std::ios_base::out | std::ios_base::app);
	m_LogFile << sEndLog << std::endl;
	m_LogFile.close();
}

void CWriteToLogFile::GetTime()
{
	time_t mcTime = time(NULL);
	localtime_s(m_pTm, &mcTime);

	m_currentTime = "";
	m_currentTime += ((m_pTm->tm_hour < 10) ? '0' + std::to_string(m_pTm->tm_hour) : std::to_string(m_pTm->tm_hour)) + ':';
	m_currentTime += ((m_pTm->tm_min < 10) ? '0' + std::to_string(m_pTm->tm_min) : std::to_string(m_pTm->tm_min)) + ':';
	m_currentTime += (m_pTm->tm_sec < 10) ? '0' + std::to_string(m_pTm->tm_sec) : std::to_string(m_pTm->tm_sec);
}

void CWriteToLogFile::GetSystemInfo()
{
	const uint32_t BUFF_SIZE = 256;

	char computerName[BUFF_SIZE];
	uint32_t computerNameSize = BUFF_SIZE;

	GetComputerName(computerName, reinterpret_cast<LPDWORD>(&computerNameSize));

	m_pSystemInfo->m_OS_NamePC = computerName;

	char userName[BUFF_SIZE];
	uint32_t userNameSize = BUFF_SIZE;

	GetUserName(userName, reinterpret_cast<LPDWORD>(&userNameSize));

	m_pSystemInfo->m_OS_NameUser = userName;

	HMODULE hMod = GetModuleHandle("ntdll.dll");

	typedef LONG NTSTATUS, * PNTSTATUS;
	typedef NTSTATUS(WINAPI* RtlGetVersionPtr)(PRTL_OSVERSIONINFOW);

	if (hMod)
	{
		RtlGetVersionPtr fxPtr = (RtlGetVersionPtr)GetProcAddress(hMod, "RtlGetVersion");
		if (fxPtr != nullptr)
		{
			RTL_OSVERSIONINFOW rovi = { 0 };
			rovi.dwOSVersionInfoSize = sizeof(rovi);
			if (SEVERITY_SUCCESS == fxPtr(&rovi))
			{
				std::string m_os;
				if (rovi.dwMajorVersion == 10 && rovi.dwMinorVersion == 0)
					m_os = "Windows 10";
				else if (rovi.dwMajorVersion == 6 && rovi.dwMinorVersion == 3)
					m_os = "Windows 8.1";
				else if (rovi.dwMajorVersion == 6 && rovi.dwMinorVersion == 2)
					m_os = "Windows 8";
				else if (rovi.dwMajorVersion == 6 && rovi.dwMinorVersion == 1)
					m_os = "Windows 7";

				m_pSystemInfo->m_OS_Version = m_os;
				m_pSystemInfo->m_OS_BuildVersion = std::to_string(rovi.dwBuildNumber);
			}
		}
	}
	RTL_OSVERSIONINFOW rovi = { 0 };


	IDXGIFactory1* pFactory;
	HRESULT hr = CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)(&pFactory));
	int AdapterNumber = 0;
	IDXGIAdapter1* Adapter;
	while (pFactory->EnumAdapters1(AdapterNumber++, &Adapter) != DXGI_ERROR_NOT_FOUND) {
		DXGI_ADAPTER_DESC1 Desc;
		Adapter->GetDesc1(&Desc);
		m_pSystemInfo->m_VideoDrivesName.push_back(Desc.Description);
		m_pSystemInfo->m_VideoDrivesMemory.push_back(std::to_string(Desc.DedicatedVideoMemory / 1024 / 1024));
		Adapter->Release();
	}
	pFactory->Release();


	int CPUInfo[4] = { -1 };
	unsigned   nExIds, i = 0;
	char CPUBrandString[0x40];

	__cpuid(CPUInfo, 0x80000000);
	nExIds = CPUInfo[0];
	for (i = 0x80000000; i <= nExIds; ++i)
	{
		__cpuid(CPUInfo, i);

		if (i == 0x80000002)
			memcpy(CPUBrandString, CPUInfo, sizeof(CPUInfo));
		else if (i == 0x80000003)
			memcpy(CPUBrandString + 16, CPUInfo, sizeof(CPUInfo));
		else if (i == 0x80000004)
			memcpy(CPUBrandString + 32, CPUInfo, sizeof(CPUInfo));
	}

	m_pSystemInfo->m_ProcessorName = CPUBrandString;

	SYSTEM_INFO sysInfo;
	GetNativeSystemInfo(&sysInfo);
	m_pSystemInfo->m_ProcessorCores = std::to_string(sysInfo.dwNumberOfProcessors);

	std::string m_Architecture;
	switch (sysInfo.wProcessorArchitecture)
	{
	case PROCESSOR_ARCHITECTURE_AMD64:
		m_Architecture = "x64(AMD or Intel)";
		break;
	case PROCESSOR_ARCHITECTURE_ARM:
		m_Architecture = "ARM";
		break;
	case PROCESSOR_ARCHITECTURE_ARM64:
		m_Architecture = "ARM64";
		break;
	case PROCESSOR_ARCHITECTURE_IA64:
		m_Architecture = "Intel Itanium - based";
		break;
	case PROCESSOR_ARCHITECTURE_INTEL:
		m_Architecture = "x86";
		break;
	case PROCESSOR_ARCHITECTURE_UNKNOWN:
		m_Architecture = "UNKNOWN";
		break;
	}
	m_pSystemInfo->m_ProcessorArchitecture = m_Architecture;

	MEMORYSTATUSEX statex;
	statex.dwLength = sizeof(statex);
	GlobalMemoryStatusEx(&statex);
	m_pSystemInfo->m_Ram = std::to_string(statex.ullTotalPhys / 1024 / 1024);

	m_pSystemInfo->m_ScreenResolution = std::to_string(GetSystemMetrics(SM_CXSCREEN)) + 'x' + std::to_string(GetSystemMetrics(SM_CYSCREEN));
}

void CWriteToLogFile::WriteSystemInfoToFile()
{
	std::fstream m_LogFile(m_path, std::ios_base::out | std::ios_base::app);
	m_LogFile << "========================================SYSTEM INFORMATION========================================" << std::endl;

	m_LogFile << "OS: " << std::endl
		<< '\t' << "PC Naming: " << m_pSystemInfo->m_OS_NamePC << std::endl
		<< '\t' << "User Naming: " << m_pSystemInfo->m_OS_NameUser << std::endl
		<< '\t' << m_pSystemInfo->m_OS_Version << std::endl
		<< '\t' << "Build Version: " << m_pSystemInfo->m_OS_BuildVersion << std::endl
		<< std::endl;

	m_LogFile << "CPU: " << std::endl
		<< '\t' << "CPU Type: " << m_pSystemInfo->m_ProcessorName << std::endl
		<< '\t' << "Number of Cores: " << m_pSystemInfo->m_ProcessorCores << std::endl
		<< '\t' << "Processor Architecture: " << m_pSystemInfo->m_ProcessorArchitecture << std::endl
		<< std::endl;

	m_LogFile << "GPU: " << std::endl;
	for (int i = 0; i < m_pSystemInfo->m_VideoDrivesName.size(); ++i)
	{
		std::string name(m_pSystemInfo->m_VideoDrivesName[i].begin(), m_pSystemInfo->m_VideoDrivesName[i].end());
		std::string memory(m_pSystemInfo->m_VideoDrivesMemory[i].begin(), m_pSystemInfo->m_VideoDrivesMemory[i].end());

		m_LogFile << '\t' << name << std::endl
			<< "\t\tMemory: " << memory << " MB" << std::endl;
	}
	m_LogFile << std::endl;

	m_LogFile << "RAM: " << m_pSystemInfo->m_Ram << " MB" << std::endl;
	m_LogFile << "Screen resolution: " << m_pSystemInfo->m_ScreenResolution << std::endl;
	m_LogFile << "========================================ENGINE INFORMATION========================================" << std::endl;

	m_LogFile << ENGINE_NAME << " v." << ENGINE_VERSION_MAJOR << '.' << ENGINE_VERSION_MINOR
		<< " BUILD " << ENGINE_VERSION_BUILD << std::endl;
	m_LogFile << ENGINE_GAME_TITLE << " v." << GAME_VERSION_MAJOR << '.' << GAME_VERSION_MINOR << std::endl;
	m_LogFile << "Build Root Directory: " << gEngine->pCore->GetRootDir() << std::endl;
	m_LogFile << "==================================================================================================" << std::endl;
	m_LogFile.close();
}
