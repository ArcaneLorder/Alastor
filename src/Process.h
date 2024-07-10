#pragma once
#include "Common.h"
#include "String.h"
#ifdef _WIN32
#include <windows.h>
#include <tlhelp32.h>
#endif

namespace Alastor 
{
	class ALASTOR_API Process 
	{
	public:
		Process();
		~Process();
	public:
		static std::vector<PROCESSENTRY32> list();
		static bool isAdmin();
		static bool isWow64(HANDLE processHandle);
		static bool exist(const String processName);
		static unsigned long getId(const String processName);
		static bool kill(unsigned long processId);
		static bool kill(const String processName);
		static void openUrl(const String url);
		static bool exec(std::string path,char* command = nullptr, bool show = true, bool wait = false);
		static BOOL setPrivilege(HANDLE hToken, LPCTSTR lpszPrivilege, BOOL bEnablePrivilege);
		static 	bool setDebugPrivilege();
		static HANDLE open(DWORD processId);
		static bool close(HANDLE processHandle);
		static unsigned int parentId(HANDLE processHandles);
		static std::string path(DWORD processId);
		static std::string command(HANDLE processHandle);

		static vector<>
	};
}