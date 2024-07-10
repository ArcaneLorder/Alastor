#include "Driver.h"
#include <Windows.h>
namespace Alastor
{
	Driver::Driver()
	{
		m_deviceHandle = NULL;
		m_error = 0;
	}

	Driver::~Driver()
	{
	}



	bool Driver::load(
		std::string path,
		std::string deviceName,
		std::string symbolName,
		unsigned long startOptions
	)
	{
		SC_HANDLE hSCManager = OpenSCManagerA(NULL, NULL, startOptions);
		if (!hSCManager)
		{
			m_error = GetLastError();
			return false;
		}
		SC_HANDLE hService = CreateServiceA(hSCManager, m_serviceName.c_str(), m_serviceName.c_str(), SERVICE_ALL_ACCESS, SERVICE_KERNEL_DRIVER, startOptions, SERVICE_ERROR_IGNORE, m_path.c_str(), 0, 0, 0, 0, 0);
		if (hService == NULL)
		{
			if (ERROR_SERVICE_EXISTS == GetLastError())
			{
				hService = OpenServiceA(hSCManager, m_serviceName.c_str(), SERVICE_ALL_ACCESS);
				if (hService == NULL)
				{
					CloseServiceHandle(hSCManager);
					m_error = GetLastError();
					return false;
				}
			}
			else 
			{
				if (ERROR_SERVICE_MARKED_FOR_DELETE == GetLastError() || ERROR_DUPLICATE_SERVICE_NAME == GetLastError())
				{
					hService = OpenServiceA(hSCManager, m_serviceName.c_str(), SERVICE_ALL_ACCESS);
					if (hService == NULL)
					{
						CloseServiceHandle(hSCManager);
						m_error = GetLastError();
						return false;
					}
					SERVICE_STATUS status;
					ControlService(hService, SERVICE_CONTROL_STOP,&status);
					DeleteService(hService);
					CloseServiceHandle(hService);
					CloseServiceHandle(hSCManager);
					hSCManager = OpenSCManagerA(NULL, NULL, SC_MANAGER_ALL_ACCESS);
					if (hSCManager == NULL)
					{
						m_error = GetLastError();
						return false;
					}

					hService = CreateServiceA(hSCManager, m_serviceName.c_str(), m_serviceName.c_str(), SERVICE_ALL_ACCESS, SERVICE_KERNEL_DRIVER, startOptions, SERVICE_ERROR_IGNORE, m_path.c_str(), 0, 0, 0, 0, 0);
					if (hService == NULL)
					{
						CloseServiceHandle(hSCManager);
						m_error = GetLastError();
						return false;
					}
				}
				else
				{
					m_path = "";
					m_deviceHandle = NULL;
					CloseServiceHandle(hSCManager);
					m_error = GetLastError();
					return false;
				}
			}
		}
		
		// 启动驱动服务
		if (StartServiceA(hService, NULL, NULL) == 0)
		{
			if (ERROR_SERVICE_ALREADY_RUNNING != GetLastError())
			{
				CloseServiceHandle(hService);
				CloseServiceHandle(hSCManager);
				m_path = "";
				m_error = GetLastError();
				return false;
			}
		}
		
		m_deviceHandle = CreateFileA(("\\\\.\\" + m_symbolName).c_str(),GENERIC_READ|GENERIC_WRITE, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,NULL);
		if (m_deviceHandle == NULL)
		{
			SERVICE_STATUS status;
			ControlService(hService, SERVICE_CONTROL_STOP, &status);
			DeleteService(hService);
			CloseServiceHandle(hService);
			CloseServiceHandle(hSCManager);

			m_path = "";
			m_error = GetLastError();
			return false;
		}

		CloseServiceHandle(hService);
		CloseServiceHandle(hSCManager);
		return true;
	}

	bool Driver::unload()
	{
		if (m_deviceHandle)
		{
			CloseHandle(m_deviceHandle);
		}

		SC_HANDLE hSCManager = OpenSCManagerA(NULL, NULL, SC_MANAGER_ALL_ACCESS);
		if (hSCManager == NULL)
		{
			m_path = "";
			m_deviceHandle = NULL;
			m_error = GetLastError();
			return false;
		}
		SC_HANDLE hService = OpenServiceA(hSCManager, m_serviceName.c_str(), SERVICE_ALL_ACCESS);
		if (hService == NULL)
		{
			CloseServiceHandle(hSCManager);
			m_path = "";
			m_deviceHandle = NULL;
			m_error = GetLastError();
			return false;
		}
		SERVICE_STATUS status;
		if (ControlService(hService, SERVICE_CONTROL_STOP, &status) == FALSE)
		{
			CloseServiceHandle(hService);
			CloseServiceHandle(hSCManager);
			m_path = "";
			m_deviceHandle = NULL;
			m_error = GetLastError();
			return false;
		}
		if (DeleteService(hService) == FALSE)
		{
			CloseServiceHandle(hService);
			CloseServiceHandle(hSCManager);
			m_path = "";
			m_deviceHandle = NULL;
			m_error = GetLastError();
			return false;
		}
		CloseServiceHandle(hService);
		CloseServiceHandle(hSCManager);
		m_path = "";
		m_deviceHandle = NULL;
		return false;
	}

	int Driver::getControlCode(int function, int deviceType, int method, int access)
	{
		return (deviceType << 2) + (access << 14) + (function << 2) + method;
	}

	bool Driver::control(int code, void* input, int inputLength, void* output, int outputLength)
	{
		OVERLAPPED overlapped;
		return DeviceIoControl(m_deviceHandle, code, input, inputLength, output, outputLength, NULL, &overlapped)==1;
	}

	std::string Driver::lasterror()
	{
		return std::string();
	}





	typedef enum _SYSTEM_INFORMATION_CLASS
	{
		SystemBasicInformation,        //  0
		SystemProcessorInformation,        //  1
		SystemPerformanceInformation,        //  2
		SystemTimeOfDayInformation,        //  3
		SystemPathInformation,        //  4
		SystemProcessInformation,               //5
		SystemCallCountInformation,        //  6
		SystemDeviceInformation,        //  7
		SystemProcessorPerformanceInformation,        //  8
		SystemFlagsInformation,        //  9
		SystemCallTimeInformation,        //  10
		SystemModuleInformation,        //  11
		SystemLocksInformation,        //  12
		SystemStackTraceInformation,        //  13
		SystemPagedPoolInformation,        //  14
		SystemNonPagedPoolInformation,        //  15
		SystemHandleInformation,        //  16
		SystemObjectInformation,        //  17
		SystemPageFileInformation,        //  18
		SystemVdmInstemulInformation,        //  19
		SystemVdmBopInformation,        //  20
		SystemFileCacheInformation,        //  21
		SystemPoolTagInformation,        //  22
		SystemInterruptInformation,        //  23
		SystemDpcBehaviorInformation,        //  24
		SystemFullMemoryInformation,        //  25
		SystemLoadGdiDriverInformation,        //  26
		SystemUnloadGdiDriverInformation,        //  27
		SystemTimeAdjustmentInformation,        //  28
		SystemSummaryMemoryInformation,        //  29
		SystemMirrorMemoryInformation,        //  30
		SystemPerformanceTraceInformation,        //  31
		SystemObsolete0,        //  32
		SystemExceptionInformation,        //  33
		SystemCrashDumpStateInformation,        //  34
		SystemKernelDebuggerInformation,        //  35
		SystemContextSwitchInformation,        //  36
		SystemRegistryQuotaInformation,        //  37
		SystemExtendServiceTableInformation,        //  38
		SystemPrioritySeperation,        //  39
		SystemVerifierAddDriverInformation,        //  40
		SystemVerifierRemoveDriverInformation,        //  41
		SystemProcessorIdleInformation,        //  42
		SystemLegacyDriverInformation,        //  43
		SystemCurrentTimeZoneInformation,        //  44
		SystemLookasideInformation,        //  45
		SystemTimeSlipNotification,        //  46
		SystemSessionCreate,        //  47
		SystemSessionDetach,        //  48
		SystemSessionInformation,        //  49
		SystemRangeStartInformation,        //  50
		SystemVerifierInformation,        //  51
		SystemVerifierThunkExtend,        //  52
		SystemSessionProcessInformation,        //  53
		SystemLoadGdiDriverInSystemSpace,        //  54
		SystemNumaProcessorMap,        //  55
		SystemPrefetcherInformation,        //  56
		SystemExtendedProcessInformation,        //  57
		SystemRecommendedSharedDataAlignment,        //  58
		SystemComPlusPackage,        //  59
		SystemNumaAvailableMemory,        //  60
		SystemProcessorPowerInformation,        //  61
		SystemEmulationBasicInformation,        //  62
		SystemEmulationProcessorInformation,        //  63
		SystemExtendedHandleInformation,        //  64
		SystemLostDelayedWriteInformation,        //  65
		SystemBigPoolInformation,        //  66
		SystemSessionPoolTagInformation,        //  67
		SystemSessionMappedViewInformation,        //  68
		SystemHotpatchInformation,        //  69
		SystemObjectSecurityMode,        //  70
		SystemWatchdogTimerHandler,        //  71
		SystemWatchdogTimerInformation,        //  72
		SystemLogicalProcessorInformation,        //  73
		SystemWow64SharedInformation,        //  74
		SystemRegisterFirmwareTableInformationHandler,        //  75
		SystemFirmwareTableInformation,        //  76
		SystemModuleInformationEx,        //  77
		SystemVerifierTriageInformation,        //  78
		SystemSuperfetchInformation,        //  79
		SystemMemoryListInformation,        //  80
		SystemFileCacheInformationEx,        //  81
		MaxSystemInfoClass                      //82

	} SYSTEM_INFORMATION_CLASS;

	typedef NTSTATUS(*lpfnZwQuerySystemInformation)(ULONG, PVOID,ULONG,PULONG);
	NTSTATUS ZwQuerySystemInformation(
		ULONG SystemInformationClass,
		PVOID SystemInformation,
		ULONG SystemInformationLength,
		PULONG ReturnLength
	)
	{
		HMODULE user32 = LoadLibrary(TEXT("ntdll.dll"));
		if (!user32)
		{
			return STATUS_DLL_INIT_FAILED;
		}
		lpfnZwQuerySystemInformation fnZwQuerySystemInformation = (lpfnZwQuerySystemInformation)GetProcAddress(user32,"ZwQuerySystemInformation");
		if (!fnZwQuerySystemInformation)
		{
			return STATUS_INVALID_HANDLE;
		}

		return fnZwQuerySystemInformation(SystemInformationClass, SystemInformation, SystemInformationLength, ReturnLength);
	}


	std::vector<SYSTEM_MODULE_INFORMATION_ENTRY> Driver::list()
	{
		std::vector<SYSTEM_MODULE_INFORMATION_ENTRY> list;


		ULONG ulRet = 0;
		NTSTATUS status = ZwQuerySystemInformation(SystemModuleInformation, NULL, NULL,&ulRet);

		if (ulRet)
		{
			
			PSYSTEM_MODULE_INFORMATION pSysModInfo = (PSYSTEM_MODULE_INFORMATION)malloc(ulRet);
			if (!pSysModInfo)
			{
				return  std::vector<SYSTEM_MODULE_INFORMATION_ENTRY>();
			}
			RtlZeroMemory(pSysModInfo, ulRet);
			status = ZwQuerySystemInformation(SystemModuleInformation, pSysModInfo, ulRet, &ulRet);
			if (BCRYPT_SUCCESS(status))
			{
				for (ULONG i = 0; i < pSysModInfo->Count; i++)
				{
					list.push_back(pSysModInfo->Module[i]);
				}

				return list;
			}
		}


		return std::vector<SYSTEM_MODULE_INFORMATION_ENTRY>();
	}
}

