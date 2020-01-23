#ifndef _I_SURFACE_H_
#define _I_SURFACE_H_

#ifdef WIN32
	#include <Windows.h>
#endif // WIN32

struct ISurface
{
#ifdef WIN32
	ISurface(HWND hwnd) : m_hwnd(hwnd) {}
	
	HWND GetHandle() { return m_hwnd; };
private:
	HWND m_hwnd = nullptr;
#endif // WIN32

	~ISurface() {}
};

#endif // _I_SURFACE_H_