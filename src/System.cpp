#include "System.h"


namespace Alastor{



	bool System::isWow64()
	{
#ifdef _WIN32
		SYSTEM_INFO si = { 0 };
		GetNativeSystemInfo(&si);

		if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64
			|| si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_IA64)
		{
			return true;
		}
		else
		{
			return false;
		}
#endif


	}
	

	std::string System::version()
	{
		NTSTATUS(WINAPI * RtlGetVersion)(LPOSVERSIONINFOEXW);

		*(FARPROC*)&RtlGetVersion = GetProcAddress(GetModuleHandle(TEXT("ntdll.dll")), "RtlGetVersion");
		if (!RtlGetVersion)
		{
			return "";
		}
		SYSTEM_INFO info;
		GetSystemInfo(&info);
		OSVERSIONINFOEXW os;
		os.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEXW);
		std::string winver;
		if (RtlGetVersion(&os) == 0)
		{
			//info.Major = osv.dwMajorVersion;
			//info.Minor = osv.dwMinorVersion;
			//info.BuildNum = osv.dwBuildNumber;
			//if (osv.dwBuildNumber >= 22000)
			//	osv.dwMajorVersion = 11;

			if (os.dwMajorVersion == 10 && os.dwMinorVersion == 0) {
				if (os.dwPlatformId == VER_PLATFORM_WIN32_NT) {
					if (os.dwBuildNumber >= 22000) winver = "Windows 11";
					else winver = "Windows 10";
				}
				else {
					switch (os.dwBuildNumber) {
					case 14393: winver = "Windows Server 2016"; break;
					case 17763:
					case 18363:
					case 19041:
					case 19042: winver = "Windows Server 2019"; break;
					case 20348: winver = "Windows Server 2022"; break;
					}
				}
			}
			else {
				switch (os.dwMajorVersion) {
				case 4:
					switch (os.dwMinorVersion) {
					case 0:
						if (os.dwPlatformId == VER_PLATFORM_WIN32_NT) winver = "Windows NT 4.0";
						else if (os.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS) winver = "Windows 95";
						break;
					case 10: winver = "Windows 98"; break;
					case 90: winver = "Windows ME"; break;
					}
					break;
				case 5:
					switch (os.dwMinorVersion) {
					case 0: winver = "Windows 2000"; break;
					case 1: winver = "Windows XP"; break;
					case 2:
						if (os.wProductType == VER_NT_WORKSTATION && info.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64)
							winver = "Windows XP Professional x64 Edition";
						else if (GetSystemMetrics(SM_SERVERR2) == 0)
							winver = "Windows Server 2003";
						else if (GetSystemMetrics(SM_SERVERR2) != 0)
							winver = "Windows Server 2003 R2";
						break;
					}
					break;
				case 6:
					switch (os.dwMinorVersion) {
					case 0:
						if (os.wProductType == VER_NT_WORKSTATION) winver = "Windows Vista";
						else winver = "Windows Server 2008";
						break;
					case 1:
						if (os.wProductType == VER_NT_WORKSTATION) winver = "Windows 7";
						else winver = "Windows Server 2008 R2";
						break;
					case 2:
						if (os.wProductType == VER_NT_WORKSTATION) winver = "Windows 8";
						else winver = "Windows Server 2012";
						break;
					case 3:
						if (os.wProductType == VER_NT_WORKSTATION) winver = "Windows 8.1";
						else winver = "Windows Server 2012 R2";
					}
					break;
				}
			}
		}
		return winver;
	}










}


